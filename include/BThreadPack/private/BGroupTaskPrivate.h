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
 * @File        : BGroupTaskPrivate.h
 * @Date        : 2019-3-21
*/

#ifndef _BGROUP_TASK_PRIVATE_H_
#define _BGROUP_TASK_PRIVATE_H_

#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>

#include "BUtils/BTimer.h"
#include "BUtils/BUtils.h"
#include "BThreadPack/BThreadPack.h"
#include "BThreadPack/BAbstractTask.h"
#include "BThreadPack/BGroupTask.h"

namespace BThreadPack{

class BGroupTaskPrivate{
public:
    BGroupTaskPrivate();
    ~BGroupTaskPrivate();

    bool queueEmpty();
    void startExecutionTiming();
    void stopExecutionTiming();
    void startRealTiming();
    void stopRealTiming();
    void setUUID();
    void setUUID(std::string &_uuid);
    void setTaskNum(int task_num);
    void setStatus(BGroupTask::BGroupTaskStatus group_status);
    void pushResultTask(BAbstractTask* &result_task_ptr);
    void finishedOneTask();
    BAbstractTask* getTask();
    BGroupTask::BGroupTaskStatus status();
    
    BTimer m_real_timer;
    BTimer m_execute_timer;
    std::atomic_int m_task_priority;
    std::atomic_int m_task_num;
    std::atomic_int m_finished_task_counter;
    std::string m_uuid;
    std::queue<BAbstractTask*> m_task_queue;
    std::queue<BAbstractTask*> m_result_task_queue;
    std::mutex m_group_task_mutex;
    std::condition_variable m_group_task_cond;
    BGroupTask::BGroupTaskStatus m_group_task_status;
};

};

#endif
