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

#include "BThreadPack/BGeneralThreadPool.h"
#include "BThreadPack/private/BGroupTaskPrivate.h"

namespace BThreadPack{

BGeneralThreadPool::BGeneralThreadPool(unsigned int _thread_cap)
    :BAbstractThreadPool(_thread_cap)
{
	m_init_(this);
}

BGeneralThreadPool::BGeneralThreadPool(unsigned int _thread_cap,
    BAbstractThreadPool::BThreadControlMode _mode)
    :BAbstractThreadPool(_thread_cap, _mode)
{
	m_init_(this);
}

BGeneralThreadPool::~BGeneralThreadPool()
{
}

int BGeneralThreadPool::m_init_(BGeneralThreadPool* _thread_pool_handle)
{
	return m_init_(_thread_pool_handle, capacity());
}

int BGeneralThreadPool::m_init_(BGeneralThreadPool* _thread_pool_handle, unsigned int _thread_num)
{
	kill();
	
	setStatus(BThreadPoolStatus::ThreadPoolRunning);
	
    for (unsigned int i = 0; i < _thread_num; ++i) {
        if(addThread(thread(BGeneralThreadPool::m_threadFunction_, _thread_pool_handle)) == BCore::ReturnCode::BThreadPoolFull)
            return BCore::ReturnCode::BThreadPoolFull;
    }
    
    // Attention: This function call can be removed.
    setAffinity();
    
    detach();
    
    return BCore::ReturnCode::BSuccess;
}

void BGeneralThreadPool::pushTask(BGeneralTask* _task_buffer)
{
    static_cast<BGeneralTask*>(_task_buffer)->startRealTiming();
    
    BAbstractThreadPool::pushTask(_task_buffer);
}

void BGeneralThreadPool::pushGroupTask(BGroupTask* _task_ptr)
{
    static_cast<BGroupTask*>(_task_ptr)->m_private_ptr->startRealTiming();
    
    BAbstractThreadPool::pushGroupTask(_task_ptr);
}

unsigned int BGeneralThreadPool::resize(unsigned int _size)
{
    if (_size < size())
    {
        return removeThread(size() - _size);
    }
    else if (_size > size())
    {
        unsigned int _thread_num = _size - size();
        for (unsigned int i = 0; i < _thread_num; ++i) {
            if(addThread(thread(BGeneralThreadPool::m_threadFunction_, this)) == BCore::ReturnCode::BThreadPoolFull)
                return BCore::ReturnCode::BThreadPoolFull;
                
            setAffinity(i + size());
        }
        detach();
        return size();
    }
    else
        return size();
}

void BGeneralThreadPool::setOptimizePolicy(BGeneralThreadPool::Optimizer _policy)
{
    m_optimize_policy = _policy;
}

const BGeneralThreadPool::Optimizer BGeneralThreadPool::optimizePolicy() const
{
    return m_optimize_policy;
}

int BGeneralThreadPool::optimizer(vector<BGeneralTask *> _task_vec, BGeneralThreadPool::Optimizer _op_policy)
{
    if (mode() != BAbstractThreadPool::DynamicThreadCapacity)
    {
#ifdef _B_DEBUG_
        B_PRINT_DEBUG("BGeneralThreadPool::BOptimizer - Thread mode "<<mode()<<" is incorrect for BOptimizer.")
#endif
        return BCore::ReturnCode::BModeIncorrect;
    }
    
    BGeneralThreadPool::setOptimizePolicy(_op_policy);
    
    switch (_op_policy) {
    	case PerformanceFirst : {
    		if(m_normalOptimizer_(_task_vec) != BCore::ReturnCode::BSuccess)
    		{
#ifdef _B_DEBUG_
        B_PRINT_DEBUG("BGeneralThreadPool::BOptimizer - m_normalOptimizer_ failed.")
#endif
				return BCore::ReturnCode::BError;
    		} else {
    			if (size() == m_max_performance_threads_)
    			{
    				return BCore::ReturnCode::BSuccess;
    			} else {
    				return resize(m_max_performance_threads_);
    			}
    		}
    	}
    	
    	case ProcessTimeFirst : {
    		if(m_normalOptimizer_(_task_vec) != BCore::ReturnCode::BSuccess)
    		{
#ifdef _B_DEBUG_
        B_PRINT_DEBUG("BGeneralThreadPool::BOptimizer - m_normalOptimizer_ failed.")
#endif
				return BCore::ReturnCode::BError;
    		} else {
    			if (size() == m_min_time_threads_)
    			{
    				return BCore::ReturnCode::BSuccess;
    			} else {
    				return resize(m_min_time_threads_);
    			}
    		}
    	}
    	default:
    		return BCore::ReturnCode::BError;
    }
    
    return BCore::ReturnCode::BSuccess;
}

void BGeneralThreadPool::m_threadFunction_(BGeneralThreadPool* _thread_pool_handle)
{   
    while(1)
    {
        // Get task and validate this task
        BGeneralTask* p_general_task = static_cast<BGeneralTask*>(_thread_pool_handle->getTask());
        
        if(p_general_task == nullptr)
        {
            _thread_pool_handle->wait();
            
            if(_thread_pool_handle->status() == BAbstractThreadPool::BThreadPoolStatus::ThreadPoolStop
                or _thread_pool_handle->isRemove())
            {
                _thread_pool_handle->threadExit(std::this_thread::get_id());
            	return;
            }else
            	continue;
        }
        // Finished validate
        
        // Set this thread namespace
        if (p_general_task->name().empty())
        {
            BGeneralThreadPool::m_setThreadName("BThreadPack");
        } else {
            BGeneralThreadPool::m_setThreadName(p_general_task->name());
        }
        
        // Processing this task and recording time
        p_general_task->setStatus(BGeneralTask::BTaskStatus::TaskExecuting);
        p_general_task->startExecutionTiming();
        
        int _retcode = p_general_task->execute();
        
        p_general_task->stopExecutionTiming();
    	p_general_task->stopRealTiming();
        // Stop processing
        
        // Check if this task has been processed successfully
        if(_retcode == BCore::ReturnCode::BError)
        {
        	p_general_task->setStatus(BGeneralTask::BTaskStatus::TaskFailed);
        }else{
        	p_general_task->setStatus(BGeneralTask::BTaskStatus::TaskFinished);
        }
        
        switch(_thread_pool_handle->optimizePolicy()){
            case PerformanceFirst:{
                unsigned long long *_task_time = new unsigned long long;
            	*_task_time = p_general_task->executionTime();
            	_thread_pool_handle->sendMessage(TASK_TIME_QUEUE, static_cast<void*>(_task_time));
            	break;
            };
            
            case ProcessTimeFirst:{
                unsigned long long *_task_time = new unsigned long long;
            	*_task_time = p_general_task->executionTime();
            	_thread_pool_handle->sendMessage(TASK_TIME_QUEUE, static_cast<void*>(_task_time));
            	break;
            };
            
            default:break;
        }
        
    	if (p_general_task->destroyable())
    	    delete p_general_task;
        // Finished check
    }
}

int BGeneralThreadPool::m_normalOptimizer_(vector<BGeneralTask *> _task_vec)
{
    //Testing the maximum threads for minimum single task processing time first.
    vector<long long> _average_time;
    vector<unsigned long long> _thread_pool_time;
    
    for(unsigned int i=0;i < capacity();i++)
    {
		BTimer _pool_timer;
        long long _fake_time = 0;
        _average_time.push_back(_fake_time);
        // Find the average time of processing a single task with different threas
        while(getTask() != nullptr)
        {
            // Clear the task queue.The thread pool is waiting for start signal now.
        }
        
        resize(i+1);
        
        for(vector<BGeneralTask *>::iterator it = _task_vec.begin(); it != _task_vec.end(); it++)
        {
            pushTask(static_cast<BGeneralTask*>(*it));
        }
        
        startAllTasks();
        
        _pool_timer.start();
        while(m_task_time_vec.size() != _task_vec.size())
        {
        	unsigned long long *_task_time = nullptr;
        	while(1)
        	{
        		_task_time = static_cast<unsigned long long *>(message(TASK_TIME_QUEUE));
        		if(_task_time == nullptr)
        		{
        			break;
        		} else {
        			m_task_time_vec.push_back(*_task_time);
        			delete _task_time;
        			break;
        		}
        	}
        }
        _pool_timer.stop();
        _thread_pool_time.push_back(_pool_timer.time());
        
        for(unsigned int j=0;j < m_task_time_vec.size();j++)
        {
            _average_time[i] += (static_cast<long long>(m_task_time_vec[j]) - _average_time[i]) / (j+1);
        }
        
        m_task_time_vec.clear();
    }
    // Finished finding the average time of processing a single task with different threas
    
    m_min_time_threads_ = 1;
    m_max_performance_threads_ = 1;
    long long _min_task_time = _average_time[0];
    unsigned long long _min_pool_time = _thread_pool_time[0];
    
    for(unsigned int i=1;i < _average_time.size();i++)
    {
        _min_task_time = _average_time[i] <= (_min_task_time + 100)?_average_time[i]:_min_task_time;
        m_min_time_threads_ = _average_time[i] <= (_min_task_time + 100)?(i+1):m_min_time_threads_;
        
        _min_pool_time = _thread_pool_time[i] <= (_min_pool_time)?_thread_pool_time[i]:_min_pool_time;
        m_max_performance_threads_ = _thread_pool_time[i] <= (_min_pool_time)?(i+1):m_max_performance_threads_;
    }
    
#ifdef _B_DEBUG_
    B_PRINT_DEBUG("BGeneralThreadPool::m_normalOptimizer_ - Threads for minimum single task processing time is "<<m_min_time_threads_<<".")
    B_PRINT_DEBUG("Result of average task processing time:")
    B_PRINT_DEBUG("Thread number| Average processing time")
    for(unsigned int i=0;i < _average_time.size();i++)
    {
        B_PRINT_DEBUG("\t"<<i+1<<"\t| "<<_average_time[i]<<" us")
    }
    B_PRINT_DEBUG("BGeneralThreadPool::m_normalOptimizer_ - Threads for maximum thread pool performance is "<<m_max_performance_threads_<<".")
    B_PRINT_DEBUG("Result of thread pool processing time:")
    B_PRINT_DEBUG("Thread number| Processing time")
    for(unsigned int i=0;i < _thread_pool_time.size();i++)
    {
        B_PRINT_DEBUG("\t"<<i+1<<"\t| "<<_thread_pool_time[i]<<" us")
    }
#endif
    
    return BCore::ReturnCode::BSuccess;
}

void BGeneralThreadPool::m_setThreadName(const char* _name)
{
#ifdef WIN32
#else
    prctl(PR_SET_NAME, _name);
#endif
}

void BGeneralThreadPool::m_setThreadName(const std::string _name)
{
    BGeneralThreadPool::m_setThreadName(_name.c_str());
}

};
