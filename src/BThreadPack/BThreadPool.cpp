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
#include "BThreadPack/private/BThreadPoolPrivate.h"
#include "BThreadPack/BThreadPool.h"
#include "BThreadPack/private/BGroupTaskPrivate.h"

namespace BThreadPack{

BThreadPool::BThreadPool(unsigned int _thread_cap)
    : BAbstractThreadPool(_thread_cap),
      m_private_ptr(new BThreadPoolPrivate(this))
{
    m_private_ptr->initializeThreadPool();
}

BThreadPool::BThreadPool(unsigned int _thread_cap,
    BAbstractThreadPool::BThreadControlMode _mode)
    : BAbstractThreadPool(_thread_cap, _mode),
      m_private_ptr(new BThreadPoolPrivate(this))
{
    m_private_ptr->initializeThreadPool();
}

BThreadPool::~BThreadPool()
{
}

void BThreadPool::pushTask(BGeneralTask* _task_buffer)
{
    static_cast<BGeneralTask*>(_task_buffer)->startRealTiming();
    
    BAbstractThreadPool::pushTask(_task_buffer);
}

void BThreadPool::pushGroupTask(BGroupTask* _task_ptr)
{
    _task_ptr->m_private_ptr->startRealTiming();
    _task_ptr->m_private_ptr->setStatus(BGroupTask::BGroupTaskStatus::Pending);
    _task_ptr->m_private_ptr->setTaskNum(_task_ptr->m_private_ptr->m_task_queue.size());
    
    BAbstractThreadPool::pushGroupTask(_task_ptr);
}

unsigned int BThreadPool::resize(unsigned int _size)
{
    if (_size < size())
    {
        return m_private_ptr->removeThread(size() - _size);
    }
    else if (_size > size())
    {
        unsigned int _thread_num = _size - size();
        for (unsigned int i = 0; i < _thread_num; ++i) {
            BThreadInfo thread_info;
            BThread bthread;
            thread_info.setThreadPoolHandle(static_cast<BAbstractThreadPool*>(this));
            bthread.start(BAbstractThreadPoolPrivate::Run, thread_info);
            // You can add some actions like set affinity here before detach.
            bthread.detach();

            return m_private_ptr->addThread(std::move(bthread));
        }
        return size();
    }
    else
        return size();
}

void BThreadPool::setOptimizePolicy(BThreadPool::Optimizer _policy)
{
    m_optimize_policy = _policy;
}

const BThreadPool::Optimizer BThreadPool::optimizePolicy() const
{
    return m_optimize_policy;
}

int BThreadPool::optimizer(std::vector<BGeneralTask *> _task_vec, BThreadPool::Optimizer _op_policy)
{
    if (mode() != BAbstractThreadPool::DynamicThreadCapacity)
    {
#ifdef _B_DEBUG_
        B_PRINT_DEBUG("BThreadPool::BOptimizer - Thread mode "<<mode()<<" is incorrect for BOptimizer.")
#endif
        return BCore::ReturnCode::BModeIncorrect;
    }
    
    BThreadPool::setOptimizePolicy(_op_policy);
    
    switch (_op_policy) {
    	case PerformanceFirst : {
    		if(m_private_ptr->normalOptimizer(_task_vec) != BCore::ReturnCode::BSuccess)
    		{
#ifdef _B_DEBUG_
        B_PRINT_DEBUG("BThreadPool::BOptimizer - m_normalOptimizer_ failed.")
#endif
				return BCore::ReturnCode::BError;
    		} else {
    			if (size() == m_private_ptr->m_max_performance_threads_)
    			{
    				return BCore::ReturnCode::BSuccess;
    			} else {
    				return resize(m_private_ptr->m_max_performance_threads_);
    			}
    		}
    	}
    	
    	case ProcessTimeFirst : {
    		if(m_private_ptr->normalOptimizer(_task_vec) != BCore::ReturnCode::BSuccess)
    		{
#ifdef _B_DEBUG_
        B_PRINT_DEBUG("BThreadPool::BOptimizer - m_normalOptimizer_ failed.")
#endif
				return BCore::ReturnCode::BError;
    		} else {
    			if (size() == m_private_ptr->m_min_time_threads_)
    			{
    				return BCore::ReturnCode::BSuccess;
    			} else {
    				return resize(m_private_ptr->m_min_time_threads_);
    			}
    		}
    	}
    	default:
    		return BCore::ReturnCode::BError;
    }
}

};
