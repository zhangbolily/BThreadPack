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
 * @File        : BAbstractThreadPoolPrivate.h
 * @Date        : 2019-3-23
*/

#ifndef INCLUDE_BTHREADPACK_PRIVATE_BABSTRACTTHREADPOOLPRIVATE_H_
#define INCLUDE_BTHREADPACK_PRIVATE_BABSTRACTTHREADPOOLPRIVATE_H_

#include <atomic>
#include <condition_variable>
#include <chrono>
#include <functional>
#include <map>
#include <queue>
#include <thread>
#include <vector>

#include "BThreadPack/BThreadPack.h"
#include "BThreadPack/BThread.h"
#include "BThreadPack/BAbstractTask.h"
#include "BThreadPack/BGroupTask.h"

namespace BThreadPack {

using std::map;
using std::atomic;
using std::atomic_int;
using std::atomic_uint;
using std::atomic_bool;
using std::vector;
using std::queue;
using std::mutex;
using std::thread;
using std::condition_variable;

class BAbstractThreadPool;

class BAbstractThreadPoolPrivate {
 public:
    explicit BAbstractThreadPoolPrivate(BAbstractThreadPool* ptr);
    explicit BAbstractThreadPoolPrivate(
            const BAbstractThreadPoolPrivate& private_obj);
    virtual ~BAbstractThreadPoolPrivate();

    BAbstractThreadPool* m_public_ptr;

    atomic_int m_priority_state;
    atomic_int m_pool_mode_;
    atomic_int m_pool_status_;
    atomic_int m_remove_count;
    atomic_uint m_thread_capacity_;
    std::vector<BThread> m_thread_vec_;
    std::vector<std::queue<BAbstractTask *>> m_priority_task_queue;
    std::vector<std::queue<BGroupTask *>> m_priority_group_task_queue;
    std::vector<bool> m_task_bitmap;
    std::vector<int> m_task_counter;
    std::queue<BAbstractTask *> m_finished_task_queue;
    mutex m_task_mutex_;
    mutex m_finished_task_mutex;
    mutex m_message_mutex_;
    mutex m_start_mutex_;
    condition_variable m_start_condition_;
    map<int, queue<void *>> m_message_queue_map_;
    map<int, condition_variable> m_message_cond_map_;
    map<int, mutex> m_message_mutex_map_;

    BAbstractTask* getTask();
    BAbstractTask* getGroupTask(BGroupTask** group_task_ptr);

    bool isRemove();
    void wait();
    void updatePriorityState(int task_priority);
    void pushFinishedTask(BAbstractTask* finished_task);
    void startOneTask();
    void startAllTasks();
    int64 addThread(BThread&& _bthread);
    int64 removeThread(uint _thread_num);

    virtual int64 initializeThreadPool();
    static void Run(BThreadInfo &thread_info);
};
}  // namespace BThreadPack

#endif  // INCLUDE_BTHREADPACK_PRIVATE_BABSTRACTTHREADPOOLPRIVATE_H_

