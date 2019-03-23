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

namespace BThreadPack {

BAbstractThreadPoolPrivate::BAbstractThreadPoolPrivate(BAbstractThreadPool* ptr)
    :m_priority_task_queue(PriorityNum),
    m_task_bitmap(PriorityNum),
    m_task_counter(PriorityNum),
    m_priority_state((PriorityNum + 1) / 2)
{

}

BAbstractThreadPoolPrivate::~BAbstractThreadPoolPrivate()
{
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
