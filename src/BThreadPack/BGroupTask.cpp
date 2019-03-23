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
    m_private_ptr->m_task_vec.push_back(_task_handle);
}

int BGroupTask::removeTask(unsigned int _task_id)
{
    if(_task_id >= size())
        return BCore::ReturnCode::BError;
    else {
        std::vector<BAbstractTask*>::iterator it = m_private_ptr->m_task_vec.begin();
        m_private_ptr->m_task_vec.erase(it + _task_id);
        
        return BCore::ReturnCode::BSuccess;
    }
}

BAbstractTask* BGroupTask::getTask(unsigned int _task_id)
{
    if(_task_id >= size())
        return nullptr;
    else {
        return m_private_ptr->m_task_vec[_task_id];
    }
}

unsigned int BGroupTask::size()
{
    return m_private_ptr->m_task_vec.size();
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

};
