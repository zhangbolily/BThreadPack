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
 * @File        : BGroupTask.cpp
 * @Date        : 2019-3-17
*/

#include "BThreadPack/private/BGroupTaskPrivate.h"
#include "BThreadPack/BGroupTask.h"

namespace BThreadPack{

BGroupTask::BGroupTask()
    :m_private_ptr(new BGroupTaskPrivate)
{
    m_private_ptr->setUUID();
}

BGroupTask::~BGroupTask()
{
    if(m_private_ptr != nullptr)
    {
        delete m_private_ptr;
        m_private_ptr = nullptr;
    }
}

void BGroupTask::pushTask(BAbstractTask* _task_handle)
{
    m_private_ptr->m_task_queue.push(_task_handle);
}

BAbstractTask* BGroupTask::getResultTask()
{
    if(m_private_ptr->m_result_task_queue.empty())
        return nullptr;
    else {
        BAbstractTask* result_task_ptr = m_private_ptr->m_result_task_queue.front();
        m_private_ptr->m_result_task_queue.pop();
        return result_task_ptr;
    }
}

unsigned int BGroupTask::size()
{
    return m_private_ptr->m_task_queue.size();
}

long long BGroupTask::executionTime()
{
    return m_private_ptr->m_execute_timer.time();
}

long long BGroupTask::realTime()
{
    return m_private_ptr->m_real_timer.time();
}

const std::string BGroupTask::UUID()
{
    return m_private_ptr->m_uuid;
}

void BGroupTask::setPriority(int _priority)
{
    m_private_ptr->m_task_priority = _priority;
}
    
int BGroupTask::priority() const
{
    return m_private_ptr->m_task_priority.load();
}

int BGroupTask::wait()
{
    if(m_private_ptr->status() == BGroupTask::BGroupTaskStatus::Init)
    {
        return BCore::ReturnCode::BSuccess;     // This group task is not running, won't be blocked.
    } else {
        while(1){
            // Group is running, block until all tasks are finished.
            std::unique_lock<mutex> lock(m_private_ptr->m_group_task_mutex);
            m_private_ptr->m_group_task_cond.wait(lock);
            
            if(m_private_ptr->status() != BGroupTask::BGroupTaskStatus::Finished)
                return BCore::ReturnCode::BError;
            else {
                // m_task_num was set when group task is pushing to thread pool.
                if(m_private_ptr->m_finished_task_counter == m_private_ptr->m_task_num)
                    return BCore::ReturnCode::BSuccess;
                else
                    continue;
            }
        }
    }
}

};
