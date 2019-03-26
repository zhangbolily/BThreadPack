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
    m_priority_state((PriorityNum + 1) / 2)
{

}

BAbstractThreadPoolPrivate::~BAbstractThreadPoolPrivate()
{
}

BAbstractTask* BAbstractThreadPoolPrivate::getTask()
{
    lock_guard<std::mutex> guard(m_task_mutex_);
    
    bool _has_task = false;
    
    for(int i=0;i < PriorityNum;i++)
    {
        _has_task |= m_task_bitmap[i];
    }
    
    if(_has_task)
    {
        // If group task queue is not empty, schedule it first.
        if(!m_priority_task_queue[m_priority_state - 1].empty())
        {
            BAbstractTask* _resultTask = m_priority_task_queue[m_priority_state - 1].front();
            m_priority_task_queue[m_priority_state - 1].pop();
            // Current priority queue maybe empty, force checking priority state.
            updatePriorityState(m_priority_state.load() - 1);
            
            return _resultTask;
        } else {
            return nullptr;
        }
    }else{
        return nullptr;
    }
}

BAbstractTask* BAbstractThreadPoolPrivate::getGroupTask(BGroupTask* &group_task_ptr)
{
    lock_guard<std::mutex> guard(m_task_mutex_);
    
    bool _has_task = false;
    
    for(int i=0;i < PriorityNum;i++)
    {
        _has_task |= m_task_bitmap[i];
    }
    
    if(_has_task)
    {
        // If group task queue is not empty, schedule it first.
        if(!m_priority_group_task_queue[m_priority_state - 1].empty())
        {
            // Group task schedule it't own tasks.
            group_task_ptr = m_priority_group_task_queue[m_priority_state - 1].front();
            BAbstractTask* _resultTask = group_task_ptr->m_private_ptr->getTask();
            
            // This group task has been finished.
            if(group_task_ptr->m_private_ptr->queueEmpty())
            {
                m_priority_group_task_queue[m_priority_state - 1].pop();  // Remove this group task from queue.
                // Current priority queue maybe empty, force checking priority state.
                updatePriorityState(m_priority_state.load() - 1);
                return _resultTask;
            } else {
                return _resultTask;
            }
        } else {
            return nullptr;
        }
    }else{
        return nullptr;
    }
}

void BAbstractThreadPoolPrivate::updatePriorityState(int task_priority)
{
    if(m_priority_task_queue[m_priority_state - 1].empty()
        && m_priority_group_task_queue[m_priority_state - 1].empty()
        && task_priority < m_priority_state.load())
    {
        m_task_bitmap[m_priority_state - 1] = false;
        bool has_task = false;
        
        for(int i= m_priority_state - 1;i > 0;i--)
        {
            if(m_task_bitmap[i-1])
            {
                has_task = true;
                m_priority_state = i;
                break;
            }
        }
        
        // No task in task queue. Reset m_priority_state to default value.
        if(!has_task)
            m_priority_state = (PriorityNum+1)/2;
    } else
        m_priority_state = task_priority > m_priority_state.load()?task_priority:m_priority_state.load();
}

void BAbstractThreadPoolPrivate::pushFinishedTask(BAbstractTask* finished_task)
{
    lock_guard<std::mutex> guard(m_finished_task_mutex);
    m_finished_task_queue.push(finished_task);
}

int BAbstractThreadPoolPrivate::_init_(BAbstractThreadPool* _this)
{
	m_public_ptr->setStatus(BAbstractThreadPool::BThreadPoolStatus::ThreadPoolRunning);
	    
    for (unsigned int i = 0; i < m_public_ptr->capacity(); ++i) {
        m_public_ptr->addThread(thread(BAbstractThreadPoolPrivate::m_threadFunction_, ref(_this)));
    }
    
    return BCore::ReturnCode::BSuccess;
}

void BAbstractThreadPoolPrivate::m_threadFunction_(void* _buffer)
{
    //Do nothing.
}

};