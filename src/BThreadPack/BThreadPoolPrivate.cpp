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
 * @File        : BThreadPoolPrivate.cpp
 * @Date        : 2019-03-27
*/

#include "BThreadPack/BThreadPool.h"
#include "BThreadPack/private/BThreadPoolPrivate.h"
#include "BThreadPack/private/BGroupTaskPrivate.h"

namespace  BThreadPack {

BThreadPoolPrivate::BThreadPoolPrivate(BThreadPool* ptr)
        : BAbstractThreadPoolPrivate(static_cast<BAbstractThreadPool*>(ptr)),
          m_public_ptr(ptr) {
}

void BThreadPoolPrivate::Run(BThreadInfo &thread_info) {
    thread_info.running();
#ifdef UNIX
#ifdef _B_DEBUG_
    thread_info.stopCPUTiming();
    B_PRINT_DEBUG("BThreadPoolPrivate::Run"
                  " - Creating thread cost CPU time: "
                          << thread_info.CPUTime() << " us")
#endif
#endif
    BThreadPool* thread_pool_handle =
            static_cast<BThreadPool*>(thread_info.threadPoolHandle());

    if (thread_pool_handle == nullptr) {
        thread_info.exit(BError);
        return;
    }

    BGeneralTask* p_general_task = nullptr;
    BGroupTask* p_group_task = nullptr;

    while (1) {
        // Get group task first
        p_general_task = static_cast<BGeneralTask*>(
                thread_pool_handle->m_private_ptr->getGroupTask(&p_group_task));
        if (p_general_task == nullptr) {
            p_group_task = nullptr;

            p_general_task = static_cast<BGeneralTask*>(
                    thread_pool_handle->m_private_ptr->getTask());
        }

        if (p_general_task == nullptr) {
            thread_pool_handle->m_private_ptr->wait();

            // Check the thread pool status and flag to determine weather exit.
            if (thread_pool_handle->status()
                == BAbstractThreadPool::BThreadPoolStatus::ThreadPoolStop
               || thread_pool_handle->m_private_ptr->isRemove()) {
                thread_info.exit(BSuccess);
                return;
            } else {
                continue;
            }
        }
        // Finished validate

        // Set this thread namespace
        if (p_general_task->name().empty()) {
            BThreadPoolPrivate::setThreadName("BThreadPack");
        } else {
            BThreadPoolPrivate::setThreadName(p_general_task->name());
        }

        // Processing this task and recording time
        if (p_group_task != nullptr) {
            p_group_task->m_private_ptr->startExecutionTiming();
            p_group_task->m_private_ptr->setStatus(
                    BGroupTask::BGroupTaskStatus::Executing);
        }
        p_general_task->m_private_ptr->setStatus(BGeneralTask::BTaskStatus::TaskExecuting);
        p_general_task->m_private_ptr->startExecutionTiming();

        int32 _retcode = p_general_task->execute();

        p_general_task->m_private_ptr->stopExecutionTiming();
        p_general_task->m_private_ptr->stopRealTiming();
        // Stop processing

        // All of group tasks have been finisehd, stop recording time.
        if (p_group_task != nullptr) {
            p_group_task->m_private_ptr->stopExecutionTiming();
            p_group_task->m_private_ptr->stopRealTiming();
            p_group_task->m_private_ptr->setStatus(
                    BGroupTask::BGroupTaskStatus::Finished);
            p_group_task->m_private_ptr->finishedOneTask();
        }

        // Check if this task has been processed successfully
        if (_retcode == BError) {
            p_general_task->m_private_ptr->setStatus(BGeneralTask::BTaskStatus::TaskFailed);
        } else {
            p_general_task->m_private_ptr->setStatus(BGeneralTask::BTaskStatus::TaskFinished);
        }

        switch (thread_pool_handle->optimizePolicy()) {
            case BThreadPool::PerformanceFirst:{
                uint64 *_task_time = new uint64;
                *_task_time = p_general_task->executionTime();
                thread_pool_handle->sendMessage(
                        TaskTimeMessageNum, static_cast<void*>(_task_time));
                break;
            };

            case BThreadPool::ProcessTimeFirst:{
                uint64 *_task_time = new uint64;
                *_task_time = p_general_task->executionTime();
                thread_pool_handle->sendMessage(
                        TaskTimeMessageNum, static_cast<void*>(_task_time));
                break;
            };

            default: break;
        }

        if (p_general_task->autoDelete()) {
            delete p_general_task;
            p_general_task = nullptr;
        } else {
            if (p_group_task != nullptr) {
                p_group_task->m_private_ptr->pushFinishedTask(
                        static_cast<BAbstractTask*>(p_general_task));
            } else {
                thread_pool_handle->m_private_ptr->pushFinishedTask(
                        static_cast<BAbstractTask*>(p_general_task));
            }
        }
        // Finished check
    }
}

int32 BThreadPoolPrivate::normalOptimizer(
        std::vector<BGeneralTask *> _task_vec) {
    // TODO(Ball Chang): This function need to be reimplemented.
    return BCore::ReturnCode::BSuccess;
}

int64 BThreadPoolPrivate::initializeThreadPool() {
    return initializeThreadPool(m_public_ptr->capacity());
}

int64 BThreadPoolPrivate::initializeThreadPool(uint32 _thread_num) {
    m_public_ptr->kill();
    m_public_ptr->setStatus(BThreadPool::BThreadPoolStatus::ThreadPoolRunning);

    BThreadInfo thread_info;
    thread_info.setThreadPoolHandle(
            static_cast<BAbstractThreadPool*>(m_public_ptr));

    for (uint32 i = 0; i < _thread_num; ++i) {
        //  Construct a new thread and pass it to thread pool.
        if (BAbstractThreadPoolPrivate::addThread(BThread())
            == BThreadPoolFull) {
            return BThreadPoolFull;
        } else {
            // You can add some actions like set affinity here before detach.
            m_thread_vec_.back().start(BThreadPoolPrivate::Run, thread_info);
#ifdef _B_DEBUG_
            B_PRINT_DEBUG("BThreadPoolPrivate::initializeThreadPool"
                          " - Added a new thread, id is "
                          << m_thread_vec_.back().id())
#endif
            m_thread_vec_.back().detach();
        }
    }

    return BCore::ReturnCode::BSuccess;
}

void BThreadPoolPrivate::setThreadName(const char* _name) {
#ifdef WIN32
#else
    prctl(PR_SET_NAME, _name);
#endif
}

void BThreadPoolPrivate::setThreadName(const std::string _name) {
    BThreadPoolPrivate::setThreadName(_name.c_str());
}

}  // namespace BThreadPack
