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
    :m_task_priority((PriorityNum+1)/2)
{
}

BGroupTaskPrivate::~BGroupTaskPrivate()
{
}

bool BGroupTaskPrivate::queueEmpty()
{
    return m_task_queue.empty();
}

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

BAbstractTask* BGroupTaskPrivate::getTask()
{
    BAbstractTask* result_task_ptr = m_task_queue.front();
    
    if(result_task_ptr == nullptr)
    {
        return nullptr;
    } else {
        m_task_queue.pop();
        return result_task_ptr;
    }
}

};

