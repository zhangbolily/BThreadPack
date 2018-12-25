/*
 * @Author      : Ball Chang
 * @File        : BAbstractThreadPool.cpp
 * @Date        : 2018-12-2
*/

#include "BGeneralThreadPool.h"

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

int BGeneralThreadPool::m_init_(BGeneralThreadPool* _this)
{
	return m_init_(_this, capacity());
}

int BGeneralThreadPool::m_init_(BGeneralThreadPool* _this, unsigned int _thread_num)
{
	kill();
	
	setStatus(BThreadPoolStatus::ThreadPoolRunning);
	
    for (unsigned int i = 0; i < _thread_num; ++i) {
        if(addThread(thread(BGeneralThreadPool::m_threadFunction_, _this)) == B_THREAD_POOL_IS_FULL)
            return B_THREAD_POOL_IS_FULL;
    }
    
    detach();
    
    return B_SUCCESS;
}

int BGeneralThreadPool::optimizer(vector<BGeneralTask *> _task_vec, BGeneralThreadPool::Optimizer _op_type)
{
    if (mode() != BAbstractThreadPool::DynamicThreadCapacity)
    {
#ifdef _B_DEBUG_
        B_PRINT_DEBUG("BGeneralThreadPool::BOptimizer - Thread mode "<<mode()<<" is incorrect for BOptimizer.")
#endif
        return B_MODE_INCORRECT;
    }
    
    switch (_op_type) {
    	case PerformanceFirst : {
    		if(m_normalOptimizer_(_task_vec) != B_SUCCESS)
    		{
#ifdef _B_DEBUG_
        B_PRINT_DEBUG("BGeneralThreadPool::BOptimizer - m_normalOptimizer_ failed.")
#endif
				return B_ERROR;
    		} else {
    			if (size() == m_max_performance_threads_)
    			{
    				return B_SUCCESS;
    			} else {
    				return m_init_(this, m_max_performance_threads_);
    			}
    		}
    	}
    	
    	case ProcessTimeFirst : {
    		if(m_normalOptimizer_(_task_vec) != B_SUCCESS)
    		{
#ifdef _B_DEBUG_
        B_PRINT_DEBUG("BGeneralThreadPool::BOptimizer - m_normalOptimizer_ failed.")
#endif
				return B_ERROR;
    		} else {
    			if (size() == m_min_time_threads_)
    			{
    				return B_SUCCESS;
    			} else {
    				return m_init_(this, m_min_time_threads_);
    			}
    		}
    	}
    	default:
    		return B_ERROR;
    }
    
    return B_SUCCESS;
}

void BGeneralThreadPool::m_threadFunction_(BGeneralThreadPool* _this)
{
    BTimer _task_timer;
    
    while(1)
    {
        // Get task and validate this task
        BGeneralTask* p_general_task = static_cast<BGeneralTask*>(_this->getTask());
        
        if(p_general_task == nullptr)
        {
            _this->wait();
            
            if(_this->status() == BAbstractThreadPool::BThreadPoolStatus::ThreadPoolStop)
            {
            	return;
            }else
            	continue;
        }
        // Finished validate
        
        // Processing this task and recording time
        p_general_task->setStatus(BGeneralTask::BTaskStatus::TaskProcessing);
        
        _task_timer.start();
        
        int _retcode = p_general_task->process();
        
        _task_timer.stop();
        // Stop processing
        
        // Check if this task has been processed successfully
        if(_retcode == B_ERROR)
        {
        	p_general_task->setStatus(BGeneralTask::BTaskStatus::TaskFailed);
        }else{
        	p_general_task->setStatus(BGeneralTask::BTaskStatus::TaskFinished);
        	unsigned long long *_task_time = new unsigned long long;
        	*_task_time = _task_timer.time();
        	_this->sendMessage(TASK_TIME_QUEUE, static_cast<void*>(_task_time));
        }
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
        
        m_init_(this, i+1);
        
        for(vector<BGeneralTask *>::iterator it = _task_vec.begin(); it != _task_vec.end(); it++)
        {
            if(addTask(static_cast<void*>(*it)) < 0)
                return B_ERROR;
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
    
    return B_SUCCESS;
}

};
