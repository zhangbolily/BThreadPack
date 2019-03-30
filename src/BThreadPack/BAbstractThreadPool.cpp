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
 * @File        : BAbstractThreadPool.cpp
 * @Date        : 2018-12-2
*/

#include "BThreadPack/private/BAbstractThreadPoolPrivate.h"
#include "BThreadPack/private/BGroupTaskPrivate.h"
#include "BThreadPack/BAbstractThreadPool.h"

namespace BThreadPack{

BAbstractThreadPool::BAbstractThreadPool(unsigned int _thread_cap)
    : m_private_ptr(new BAbstractThreadPoolPrivate(this)) {
      m_private_ptr->m_pool_mode_ = BThreadControlMode::FixedThreadCapacity;

    this->setCapacity(_thread_cap);

    for (int i = 0; i < PriorityNum; i++) {
        m_private_ptr->m_task_bitmap[i] = 0;
        m_private_ptr->m_task_counter[i] = 0;
    }
}

BAbstractThreadPool::BAbstractThreadPool(unsigned int _thread_cap,
    BAbstractThreadPool::BThreadControlMode _mode)
    : m_private_ptr(new BAbstractThreadPoolPrivate(this)) {
      m_private_ptr->m_thread_capacity_ = _thread_cap;
      m_private_ptr->m_pool_mode_ = _mode;

    this->setCapacity(_thread_cap);

    for (int i = 0; i < PriorityNum; i++) {
        m_private_ptr->m_task_bitmap[i] = 0;
        m_private_ptr->m_task_counter[i] = 0;
    }
}

BAbstractThreadPool::BAbstractThreadPool(
        BAbstractThreadPoolPrivate* private_ptr,
        uint _thread_cap)
        : m_private_ptr(private_ptr) {
        m_private_ptr->m_pool_mode_ = BThreadControlMode::FixedThreadCapacity;

    this->setCapacity(_thread_cap);

    for (int i = 0; i < PriorityNum; i++) {
        m_private_ptr->m_task_bitmap[i] = 0;
        m_private_ptr->m_task_counter[i] = 0;
    }
}

BAbstractThreadPool::BAbstractThreadPool(
        BAbstractThreadPoolPrivate* private_ptr,
        uint _thread_cap,
        BAbstractThreadPool::BThreadControlMode _mode)
        : m_private_ptr(private_ptr) {
        m_private_ptr->m_thread_capacity_ = _thread_cap;
        m_private_ptr->m_pool_mode_ = _mode;

    this->setCapacity(_thread_cap);

    for (int i = 0; i < PriorityNum; i++) {
        m_private_ptr->m_task_bitmap[i] = 0;
        m_private_ptr->m_task_counter[i] = 0;
    }
}

BAbstractThreadPool::~BAbstractThreadPool() {
    this->kill();
}

int BAbstractThreadPool::mode() const {
    return m_private_ptr->m_pool_mode_;
}

unsigned int BAbstractThreadPool::capacity() const {
    return m_private_ptr->m_thread_capacity_;
}

void BAbstractThreadPool::setCapacity(unsigned int _capacity) {
    switch (m_private_ptr->m_pool_mode_) {
        case BThreadControlMode::FixedThreadCapacity: {
            m_private_ptr->m_thread_capacity_ = _capacity;
            break;
        };
        case BThreadControlMode::DynamicThreadCapacity: {
            unsigned int _tempCapcity = thread::hardware_concurrency();
            m_private_ptr->m_thread_capacity_ =
                    _tempCapcity > _capacity?_capacity:_tempCapcity;
#ifdef _B_DEBUG_
            B_PRINT_DEBUG("BAbstractThreadPool::setCapacity"
                          " - Calculating dynamic thread capacity number.")
            B_PRINT_DEBUG("BAbstractThreadPool::setCapacity"
                          " - Current logistic cpu core number :"
                          << thread::hardware_concurrency())
            B_PRINT_DEBUG("BAbstractThreadPool::setCapacity"
                          " - Current thread pool capacity number :"
                          << m_private_ptr->m_thread_capacity_)
#endif
        };
        default:
            break;
    }
}

unsigned int BAbstractThreadPool::size() const {
    return m_private_ptr->m_thread_vec_.size();
}

void BAbstractThreadPool::setStatus(BThreadPoolStatus _status) {
    m_private_ptr->m_pool_status_ = static_cast<int>(_status);
}

int BAbstractThreadPool::status() const {
    return m_private_ptr->m_pool_status_;
}

int BAbstractThreadPool::kill() {
    // TODO(Ball Chang): Set timeout for zombie threads.
    this->setStatus(BThreadPoolStatus::ThreadPoolStop);
    /* Notify all threads to get the status flag, then thread will exit.*/
    this->m_private_ptr->startAllTasks();
    /* Wait until all threads exit. */
    std::chrono::milliseconds _ms_time(1000);
    bool is_exit = true;
    while (size()) {
        is_exit = true;
        for (int32 i = 0; i < size(); i++) {
            is_exit &= m_private_ptr->m_thread_vec_[i].isExit();
        }
        if (is_exit)
            break;
        // Some times a notify all won't wake up all threads
        // Need to notify them in every circle.
        this->m_private_ptr->startAllTasks();
        std::this_thread::sleep_for(_ms_time);
    }

    /* All threads exit. Delete all threads.*/
    m_private_ptr->m_thread_vec_.clear();

    return BCore::ReturnCode::BSuccess;
}

void BAbstractThreadPool::pushTask(BAbstractTask* _task_ptr) {
    m_private_ptr->m_task_mutex_.lock();
    int task_priority = _task_ptr->priority();
    m_private_ptr->m_priority_task_queue[task_priority - 1].push(_task_ptr);
    m_private_ptr->m_task_bitmap[task_priority - 1] = true;
    m_private_ptr->updatePriorityState(task_priority);
    m_private_ptr->m_task_mutex_.unlock();

    m_private_ptr->startAllTasks();
}

void BAbstractThreadPool::pushGroupTask(BGroupTask* _task_ptr) {
    m_private_ptr->m_task_mutex_.lock();
    int task_priority = _task_ptr->priority();
    m_private_ptr->
    m_priority_group_task_queue[task_priority - 1].push(_task_ptr);
    m_private_ptr->m_task_bitmap[task_priority - 1] = true;
    m_private_ptr->updatePriorityState(task_priority);
    m_private_ptr->m_task_mutex_.unlock();

    m_private_ptr->startAllTasks();
}

int BAbstractThreadPool::taskQueueSize() {
    lock_guard<std::mutex> guard(m_private_ptr->m_task_mutex_);

    int _size = 0;

    for (int i = 0; i < PriorityNum; i++) {
        _size += m_private_ptr->m_priority_task_queue[i].size();
    }

    return _size;
}

BAbstractTask* BAbstractThreadPool::getFinishedTask() {
    lock_guard<std::mutex> guard(m_private_ptr->m_finished_task_mutex);

    BAbstractTask* finished_task = m_private_ptr->m_finished_task_queue.front();
    m_private_ptr->m_finished_task_queue.pop();

    return finished_task;
}

int BAbstractThreadPool::sendMessage(int _queue_num, void* _message_buffer) {
    m_private_ptr->m_message_mutex_.lock();

    if (m_private_ptr->m_message_queue_map_.find(_queue_num)
        == m_private_ptr->m_message_queue_map_.end()) {
        m_private_ptr->m_message_queue_map_[_queue_num];
        m_private_ptr->m_message_mutex_map_[_queue_num];
        m_private_ptr->m_message_cond_map_[_queue_num];
        m_private_ptr->m_message_mutex_.unlock();
    } else {
        m_private_ptr->m_message_mutex_.unlock();
    }

    m_private_ptr->m_message_mutex_map_[_queue_num].lock();
    m_private_ptr->m_message_queue_map_[_queue_num].push(_message_buffer);
    m_private_ptr->m_message_mutex_map_[_queue_num].unlock();
    m_private_ptr->m_message_cond_map_[_queue_num].notify_all();

    return BCore::ReturnCode::BSuccess;
}

void* BAbstractThreadPool::message(int _queue_num) {
    m_private_ptr->m_message_mutex_.lock();

    if (m_private_ptr->m_message_queue_map_.find(_queue_num)
        == m_private_ptr->m_message_queue_map_.end()) {
        m_private_ptr->m_message_queue_map_[_queue_num];
        m_private_ptr->m_message_mutex_map_[_queue_num];
        m_private_ptr->m_message_cond_map_[_queue_num];
        m_private_ptr->m_message_mutex_.unlock();
    } else {
        m_private_ptr->m_message_mutex_.unlock();
    }

    if (m_private_ptr->m_message_queue_map_[_queue_num].empty()) {
        std::unique_lock<mutex> lock(m_private_ptr->
                                m_message_mutex_map_[_queue_num]);
        m_private_ptr->m_message_cond_map_[_queue_num].wait(lock);

        if (m_private_ptr->m_message_queue_map_[_queue_num].empty()) {
            m_private_ptr->m_message_mutex_map_[_queue_num].unlock();
            return nullptr;
        } else {
            void* _result_message =
                    m_private_ptr->m_message_queue_map_[_queue_num].front();
            m_private_ptr->m_message_queue_map_[_queue_num].pop();

            m_private_ptr->m_message_mutex_map_[_queue_num].unlock();
            return _result_message;
        }
    } else {
        m_private_ptr->m_message_mutex_map_[_queue_num].lock();
        void* _result_message =
                m_private_ptr->m_message_queue_map_[_queue_num].front();
        m_private_ptr->m_message_queue_map_[_queue_num].pop();

        m_private_ptr->m_message_mutex_map_[_queue_num].unlock();
        return _result_message;
    }
}

};  // namespace BThreadPack
