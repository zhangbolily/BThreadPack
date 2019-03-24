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
 * @File        : BGroupTaskPrivate.cpp
 * @Date        : 2019-3-21
*/

#include "BThreadPack/BGroupTask.h"
#include "BThreadPack/private/BGroupTaskPrivate.h"

namespace BThreadPack{

BGroupTaskPrivate::BGroupTaskPrivate()
    :m_task_priority((PriorityNum+1)/2),
    m_finished_task_counter(0),
    m_group_task_status(BGroupTask::BGroupTaskStatus::Init)
{
}

BGroupTaskPrivate::~BGroupTaskPrivate()
{
}

bool BGroupTaskPrivate::queueEmpty()
{
    return m_task_queue.empty();
}

// This function called when the group task is scheduled.
void BGroupTaskPrivate::startExecutionTiming()
{
    m_execute_timer.start();
}

void BGroupTaskPrivate::stopExecutionTiming()
{
    m_execute_timer.stop();
}

void BGroupTaskPrivate::startRealTiming()
{
    m_real_timer.start();
}

void BGroupTaskPrivate::stopRealTiming()
{
     m_real_timer.stop();
}

void BGroupTaskPrivate::setUUID()
{
    if(!BUtils::isUUID4(m_uuid))
        m_uuid = BUtils::generateUUID4();
#ifdef _B_DEBUG_
        B_PRINT_DEBUG("BGroupTaskPrivate::setUUID - UUID is " << m_uuid << " .")
#endif
}

void BGroupTaskPrivate::setUUID(std::string &_uuid)
{
    m_uuid = _uuid;
}

void BGroupTaskPrivate::setTaskNum(int task_num)
{
    m_task_num.store(task_num);
}

void BGroupTaskPrivate::setStatus(BGroupTask::BGroupTaskStatus group_status)
{
    m_group_task_status = group_status;
}

BGroupTask::BGroupTaskStatus BGroupTaskPrivate::status()
{
    return m_group_task_status;
}

void BGroupTaskPrivate::pushFinishedTask(BAbstractTask* result_task_ptr)
{
    lock_guard<std::mutex> guard(m_finished_task_mutex);
    m_result_task_queue.push(result_task_ptr);
}

BAbstractTask* BGroupTaskPrivate::getTask()
{
    lock_guard<std::mutex> guard(m_task_mutex);

    BAbstractTask* task_ptr = m_task_queue.front();
    
    if(task_ptr == nullptr)
    {
        return nullptr;
    } else {
        m_task_queue.pop();
        return task_ptr;
    }
}

void BGroupTaskPrivate::finishedOneTask()
{
    m_finished_task_counter++;
    m_group_task_cond.notify_all();
}

};

