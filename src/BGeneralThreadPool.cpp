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
	setStatus(BThreadPoolStatus::ThreadPoolRunning);
	
    for (unsigned int i = 0; i < capacity(); ++i) {
        if(addThread(thread(BGeneralThreadPool::m_threadFunction_, _this)) == B_THREAD_POOL_IS_FULL)
            return B_THREAD_POOL_IS_FULL;
    }
    
    detach();
    
    return B_SUCCESS;
}

int BGeneralThreadPool::BOptimizer(vector<BGeneralTask *> _task_vec)
{
    if (mode() != BAbstractThreadPool::DynamicThreadCapacity)
    {
#ifdef _B_DEBUG_
        B_PRINT_DEBUG("BGeneralThreadPool::BOptimizer - Thread mode "<<mode()<<" is incorrect for BOptimizer.")
#endif
        return B_MODE_INCORRECT;
    }
    
    if(m_getMaximumThreadsforSingleTaskProcessing_(_task_vec) != B_SUCCESS)
    {
#ifdef _B_DEBUG_
        B_PRINT_DEBUG("BGeneralThreadPool::BOptimizer - m_getMaximumThreadsforSingleTaskProcessing_ failed.")
#endif
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
        	lock_guard<std::mutex> guard(_this->m_task_time_mutex);
        	_this->m_task_time_vec.push_back(_task_timer.time());
        	_this->notifyOptimizer();
        }
        // Finished check
    }
}

void BGeneralThreadPool::notifyOptimizer()
{
    m_timer_condition_.notify_one();
}

int BGeneralThreadPool::m_getMaximumThreadsforSingleTaskProcessing_(vector<BGeneralTask *> _task_vec)
{
    //Testing the maximum threads for minimum single task processing time first.
    vector<unsigned long long> _task_processing_average_time;
    
    unsigned int _threads_limit = capacity();
    setCapacity(_threads_limit);
    
    kill();
    while(size()!=0);
    
    for(unsigned int i=0;i < _threads_limit;i++)
    {
#ifdef _B_DEBUG_
        //B_PRINT_DEBUG("BGeneralThreadPool::m_getMaximumThreadsforSingleTaskProcessing_ - Round "<<i<<".")
#endif
        unsigned long long _fake_time = 0;
        _task_processing_average_time.push_back(_fake_time);
        // Find the average time of processing a single task with different threas
        while(getTask() != nullptr)
        {
            // Clear the task queue.The thread pool is waiting for start signal now.
        }
        
        setStatus(BThreadPoolStatus::ThreadPoolRunning);
        
        if(size() < (i+1)){
            while(1){
                if(addThread(thread(BGeneralThreadPool::m_threadFunction_, this)) == B_SUCCESS)
                {
                    if(size() == (i+1))
                        break;
                }else {
                    break;
                }
            }
        }else if(size() > (i+1)){
            while(1){
                if(removeThread(size() - 1) == (i+1))
                    break;
            }
        }
        
        detach();
        
        for(vector<BGeneralTask *>::iterator it = _task_vec.begin(); it != _task_vec.end(); it++)
        {
            if(addTask(static_cast<void*>(*it)) < 0)
                return B_ERROR;
        }
        
        startAllTasks();
        
        while(m_task_time_vec.size() != _task_vec.size()){
            unique_lock<std::mutex> lock(m_timer_mutex_);
            m_timer_condition_.wait(lock);
        }
        
        kill();
        while(size()!=0);
        
        for(unsigned int j=0;j < m_task_time_vec.size();j++)
        {
            _task_processing_average_time[i] += (m_task_time_vec[j] - _task_processing_average_time[i]) / (j+1);
        }
        
        m_task_time_vec.clear();
    }
    // Finished finding the average time of processing a single task with different threas
    
    m_min_time_threads_ = 1;
    unsigned long long _min_time = _task_processing_average_time[0];
    
    for(unsigned int i=1;i < _task_processing_average_time.size();i++)
    {
        _min_time = _task_processing_average_time[i] <= (_min_time + 100)?_task_processing_average_time[i]:_min_time;
        m_min_time_threads_ = _task_processing_average_time[i] <= (_min_time + 100)?(i+1):m_min_time_threads_;
    }
    
#ifdef _B_DEBUG_
    B_PRINT_DEBUG("BGeneralThreadPool::m_getMaximumThreadsforSingleTaskProcessing_ - m_min_time_threads_ "<<m_min_time_threads_)
    B_PRINT_DEBUG("BGeneralThreadPool::m_getMaximumThreadsforSingleTaskProcessing_ - _min_time "<<_min_time)
#endif
    
    return B_SUCCESS;
}

};
