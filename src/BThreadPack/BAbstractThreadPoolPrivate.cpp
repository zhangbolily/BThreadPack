/* MIT License
* 
* Copyright (c) 2018 Ball Chang
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

/*
 * @Author      : Ball Chang
 * @File        : BAbstractThreadPoolPrivate.cpp
 * @Date        : 2019-3-23
*/

#include "BThreadPack/BAbstractThreadPool.h"
#include "BThreadPack/private/BAbstractThreadPoolPrivate.h"
#include "BThreadPack/private/BGroupTaskPrivate.h"

namespace BThreadPack {

BAbstractThreadPoolPrivate::BAbstractThreadPoolPrivate(BAbstractThreadPool* ptr)
        :m_priority_task_queue(PriorityNum),
         m_priority_group_task_queue(PriorityNum),
         m_task_bitmap(PriorityNum),
         m_task_counter(PriorityNum),
         m_priority_state((PriorityNum + 1) / 2),
         m_public_ptr(ptr) {
}

BAbstractThreadPoolPrivate::BAbstractThreadPoolPrivate(
        const BAbstractThreadPoolPrivate& private_obj)
        :m_priority_state(private_obj.m_priority_state.load()),
         m_pool_mode_(private_obj.m_pool_mode_.load()),
         m_pool_status_(private_obj.m_pool_status_.load()),
         m_remove_count(private_obj.m_remove_count.load()),
         m_thread_capacity_(private_obj.m_thread_capacity_.load()),
         m_thread_vec_(private_obj.m_thread_vec_),
         m_priority_task_queue(private_obj.m_priority_task_queue),
         m_priority_group_task_queue(private_obj.m_priority_group_task_queue),
         m_task_bitmap(private_obj.m_task_bitmap),
         m_task_counter(private_obj.m_task_counter),
         m_finished_task_queue(private_obj.m_finished_task_queue),
         m_message_queue_map_(private_obj.m_message_queue_map_),
         m_public_ptr(private_obj.m_public_ptr) {
}

BAbstractThreadPoolPrivate::~BAbstractThreadPoolPrivate() {
    if (!m_finished_task_queue.empty()) {
        while (m_finished_task_queue.front() != nullptr) {
            delete m_finished_task_queue.front();
            m_finished_task_queue.pop();
        }
    }
    // Delete all tasks in the queue.
    BAbstractTask* _task = nullptr;
    BGroupTask* _gtask = nullptr;
    while (1) {
        _task = getTask();
        if (_task != nullptr)
            delete _task;
        else
            break;

        _task = getGroupTask(&_gtask);
        if (_task != nullptr) {
            delete _task;
            if (_gtask->m_private_ptr->queueEmpty())
                delete _gtask;
        } else {
            break;
        }
    }
    // Delete all message in message queue.
    // TODO(Ball Chang): Message queue contains generic pointer
    // which can't be deleted.
    for (map<int, queue<void *>>::iterator it = m_message_queue_map_.begin();
        it != m_message_queue_map_.end();
        ++it) {
        while (it->second.front() != nullptr) {
            //delete it->second.front();
            it->second.pop();
        }
    }
}

BAbstractTask* BAbstractThreadPoolPrivate::getTask() {
    lock_guard<std::mutex> guard(m_task_mutex_);
    bool _has_task = false;

    for (int i = 0; i < PriorityNum; i++) {
        _has_task |= m_task_bitmap[i];
    }

    if (_has_task) {
        // If group task queue is not empty, schedule it first.
        if (!m_priority_task_queue[m_priority_state - 1].empty()) {
            BAbstractTask* _resultTask =
                    m_priority_task_queue[m_priority_state - 1].front();
            m_priority_task_queue[m_priority_state - 1].pop();
            // Current priority queue maybe empty
            // force checking priority state.
            updatePriorityState(m_priority_state.load() - 1);

            return _resultTask;
        } else {
            return nullptr;
        }
    } else {
        return nullptr;
    }
}

BAbstractTask* BAbstractThreadPoolPrivate::getGroupTask(
        BGroupTask** group_task_ptr) {
    lock_guard<std::mutex> guard(m_task_mutex_);

    bool _has_task = false;

    for (int i = 0; i < PriorityNum; i++) {
        _has_task |= m_task_bitmap[i];
    }

    if (_has_task) {
        // If group task queue is not empty, schedule it first.
        if (!m_priority_group_task_queue[m_priority_state - 1].empty()) {
            // Group task schedule it't own tasks.
            *group_task_ptr =
                    m_priority_group_task_queue[m_priority_state - 1].front();
            BAbstractTask* _resultTask =
                    (*group_task_ptr)->m_private_ptr->getTask();

            // This group task has been finished.
            if ((*group_task_ptr)->m_private_ptr->queueEmpty()) {
                // Remove this group task from queue.
                m_priority_group_task_queue
                [m_priority_state - 1].pop();
                // Current priority queue maybe empty
                // force checking priority state.
                updatePriorityState(m_priority_state.load() - 1);
                return _resultTask;
            } else {
                return _resultTask;
            }
        } else {
            return nullptr;
        }
    } else {
        return nullptr;
    }
}

bool BAbstractThreadPoolPrivate::isRemove() {
    if (m_remove_count) {
        m_remove_count--;
        return true;
    } else {
        return false;
    }
}

void BAbstractThreadPoolPrivate::updatePriorityState(int task_priority) {
    if (m_priority_task_queue[m_priority_state - 1].empty()
        && m_priority_group_task_queue[m_priority_state - 1].empty()
        && task_priority < m_priority_state.load()) {
        m_task_bitmap[m_priority_state - 1] = false;
        bool has_task = false;

        for (int i = m_priority_state - 1; i > 0; i--) {
            if (m_task_bitmap[i-1]) {
                has_task = true;
                m_priority_state = i;
                break;
            }
        }

        // No task in task queue. Reset m_priority_state to default value.
        if (!has_task)
            m_priority_state = (PriorityNum+1)/2;
    } else {
        m_priority_state = task_priority > m_priority_state.load()
                           ?task_priority:m_priority_state.load();
    }
}

void BAbstractThreadPoolPrivate::pushFinishedTask(
        BAbstractTask* finished_task) {
    lock_guard<std::mutex> guard(m_finished_task_mutex);
    m_finished_task_queue.push(finished_task);
}

int64 BAbstractThreadPoolPrivate::initializeThreadPool() {
    m_public_ptr->setStatus(
            BAbstractThreadPool::BThreadPoolStatus::ThreadPoolRunning);

    for (uint i = 0; i < m_public_ptr->capacity(); ++i) {
        //  Construct a new thread and pass it to thread pool.
        BThreadInfo thread_info;
        BThread bthread;
        thread_info.setThreadPoolHandle(m_public_ptr);
        bthread.start(BAbstractThreadPoolPrivate::Run, thread_info);
        // You can add some actions like set affinity here before detach.
        bthread.detach();

        BAbstractThreadPoolPrivate::addThread(std::move(bthread));
    }

    return BCore::ReturnCode::BSuccess;
}

int64 BAbstractThreadPoolPrivate::addThread(BThread&& _bthread) {
    if (m_public_ptr->capacity() == m_public_ptr->size()) {
#ifdef _B_DEBUG_
        B_PRINT_DEBUG("BAbstractThreadPoolPrivate::addThread"
                      " - Thread number reach the thread pool"
                      " capacity limitation.")
        B_PRINT_DEBUG("BAbstractThreadPoolPrivate::addThread"
                      " - Current thread pool capacity number :"
                      << m_thread_capacity_)
#endif
        return BCore::ReturnCode::BThreadPoolFull;
    } else {
        m_thread_vec_.push_back(std::move(_bthread));
        return m_public_ptr->size();
    }
}

void BAbstractThreadPoolPrivate::wait() {
    std::unique_lock<std::mutex> lock(m_start_mutex_);
    m_start_condition_.wait(lock);
}

void BAbstractThreadPoolPrivate::startOneTask() {
    m_start_condition_.notify_one();
}

void BAbstractThreadPoolPrivate::startAllTasks() {
    m_start_condition_.notify_all();
}

int64 BAbstractThreadPoolPrivate::removeThread(uint _thread_num) {
    if (_thread_num > m_public_ptr->size()) {
#ifdef _B_DEBUG_
        B_PRINT_DEBUG("BAbstractThreadPool::removeThread"
                      " - Thread number overload.")
#endif
        return BCore::ReturnCode::BThreadNotExists;
    } else {
        m_remove_count = _thread_num;
        startAllTasks();
        // Wait 500ms for thread exit.
        microseconds _ms_time(500);
        std::this_thread::sleep_for(_ms_time);

        for (uint i = 0; i < m_thread_vec_.size(); ) {
            if (m_thread_vec_[i].isExit()) {
                m_thread_vec_.erase(m_thread_vec_.begin() + i);
            } else {
                i++;
            }
        }
        // Remove action finished, reset m_remove_count
        m_remove_count = 0;
        return m_thread_vec_.size();
    }
}

void BAbstractThreadPoolPrivate::Run(BThreadInfo &thread_info) {
    // Do nothing.
}

};  // namespace BThreadPack
