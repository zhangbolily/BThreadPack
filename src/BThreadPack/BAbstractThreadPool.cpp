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

#include "BThreadPack/BAbstractThreadPool.h"

namespace BThreadPack{

BAbstractThreadPool::BAbstractThreadPool(unsigned int _thread_cap)
    :m_thread_capacity_(_thread_cap),
    m_priority_task_queue(PriorityNum),
    m_task_bitmap(PriorityNum),
    m_task_counter(PriorityNum),
    m_priority_state((PriorityNum+1)/2),
    m_pool_mode_(BAbstractThreadPool::BThreadControlMode::FixedThreadCapacity)
{
    this->setCapacity(_thread_cap);
    
    for(int i=0;i < PriorityNum;i++)
    {
        m_task_bitmap[i] = 0;
        m_task_counter[i] = 0;
    }
}

BAbstractThreadPool::BAbstractThreadPool(unsigned int _thread_cap,
    BAbstractThreadPool::BThreadControlMode _mode)
    :m_thread_capacity_(_thread_cap),
    m_priority_task_queue(PriorityNum),
    m_task_bitmap(PriorityNum),
    m_task_counter(PriorityNum),
    m_priority_state((PriorityNum+1)/2),
    m_pool_mode_(_mode)
    
{
    this->setCapacity(_thread_cap);
    
    for(int i=0;i < PriorityNum;i++)
    {
        m_task_bitmap[i] = 0;
        m_task_counter[i] = 0;
    }
}

BAbstractThreadPool::~BAbstractThreadPool()
{
    this->kill();
}

int BAbstractThreadPool::mode() const
{
    return m_pool_mode_;
}

unsigned int BAbstractThreadPool::capacity() const
{
    return m_thread_capacity_;
}

void BAbstractThreadPool::setCapacity(unsigned int _capacity)
{
    switch(m_pool_mode_){
        case BThreadControlMode::FixedThreadCapacity:{
            m_thread_capacity_ = _capacity;
            break;
        };
        case BThreadControlMode::DynamicThreadCapacity:{
            unsigned int _tempCapcity = thread::hardware_concurrency();
            m_thread_capacity_ = _tempCapcity > _capacity?_capacity:_tempCapcity;
#ifdef _B_DEBUG_
            B_PRINT_DEBUG("BAbstractThreadPool::setCapacity - Calculating dynamic thread capacity number.")
            B_PRINT_DEBUG("BAbstractThreadPool::setCapacity - Current logistic cpu core number :"<<thread::hardware_concurrency())
            B_PRINT_DEBUG("BAbstractThreadPool::setCapacity - Current thread pool capacity number :"<<m_thread_capacity_)
#endif
        };
        default:
            break;
    }
}

unsigned int BAbstractThreadPool::size() const
{
    return m_thread_vec_.size();
}

long long BAbstractThreadPool::addThread(thread _new_thread)
{
    if(this->capacity() == this->size())
    {
#ifdef _B_DEBUG_
        B_PRINT_DEBUG("BAbstractThreadPool::addThread - Thread number reach the thread pool capacity limitation.")
        B_PRINT_DEBUG("BAbstractThreadPool::addThread - Current thread pool capacity number :"<<m_thread_capacity_)
#endif
        return BCore::ReturnCode::BThreadPoolFull;
    }
    else{
        m_thread_vec_.push_back(std::move(_new_thread));
        m_thread_id_vec.push_back(m_thread_vec_.back().get_id());
        m_thread_exit_map[m_thread_vec_.back().get_id()] = false;
#ifdef _B_DEBUG_
        B_PRINT_DEBUG("BAbstractThreadPool::addThread - Added a new thread, id is "<<m_thread_vec_.back().get_id())
#endif
        return this->size();
    }
    return BCore::ReturnCode::BSuccess;
}

long long BAbstractThreadPool::setAffinity()
{
    int _retcode = 0;
    
    for(unsigned int i=0;i < size();i++)
    {
        _retcode = setAffinity(i);
        
        if(_retcode < 0)
            return _retcode;
    }
    
    return _retcode;
}

long long BAbstractThreadPool::setAffinity(unsigned int _thread_num)
{
    if(_thread_num >= size())
        return BCore::ReturnCode::BParameterOutOfRange;
    
    cpu_set_t _cpuset;
    CPU_ZERO(&_cpuset);
    
    int _retcode = 0;
    unsigned int _cpus = thread::hardware_concurrency();
    unsigned int _cpu_num = _thread_num % _cpus;
    
    CPU_SET(_cpu_num, &_cpuset);
    
    _retcode = pthread_setaffinity_np(m_thread_vec_[_thread_num].native_handle(),
                                    sizeof(cpu_set_t), &_cpuset);
    if(_retcode != 0)
        return BCore::ReturnCode::BError;
    else
        return BCore::ReturnCode::BSuccess;
}

long long BAbstractThreadPool::removeThread(unsigned int _thread_num)
{
	if(_thread_num > this->size())
    {
#ifdef _B_DEBUG_
        B_PRINT_DEBUG("BAbstractThreadPool::removeThread - Thread number overload.")
#endif
        return BCore::ReturnCode::BThreadNotExists;
    }
    else{
        m_remove_count.store(_thread_num);
        startAllTasks();
        //Wait 500ms
        microseconds _ms_time(500);
        this_thread::sleep_for(_ms_time);
        
        for(unsigned int i=0;i < m_thread_vec_.size();)
        {
            if(isThreadExit(m_thread_id_vec[i]))
            {
#ifdef _B_DEBUG_
        B_PRINT_DEBUG("BAbstractThreadPool::removeThread - Removed a thread, id is "<<m_thread_id_vec[i])
#endif
                m_thread_exit_map.erase(m_thread_id_vec[i]);
                m_thread_vec_.erase(m_thread_vec_.begin() + i);
                m_thread_id_vec.erase(m_thread_id_vec.begin() + i);
            }
            else
                i++;
        }
        return this->size();
    }
    return BCore::ReturnCode::BSuccess;
}

bool BAbstractThreadPool::isRemove()
{
    if (m_remove_count)
    {
        m_remove_count--;
        return true;
    } else {
        return false;
    }
}

void BAbstractThreadPool::threadExit(std::thread::id _tid)
{
    lock_guard<std::mutex> guard(m_thread_exit_mutex);
    m_thread_exit_map[_tid] = true;
}

bool BAbstractThreadPool::isThreadExit(std::thread::id _tid)
{
    lock_guard<std::mutex> guard(m_thread_exit_mutex);
    return m_thread_exit_map[_tid];
}

int BAbstractThreadPool::_init_(BAbstractThreadPool* _this)
{
	this->setStatus(BThreadPoolStatus::ThreadPoolRunning);
	    
    for (unsigned int i = 0; i < this->capacity(); ++i) {
        this->addThread(thread(BAbstractThreadPool::m_threadFunction_, ref(_this)));
    }
    
    return BCore::ReturnCode::BSuccess;
}

int BAbstractThreadPool::join()
{
	for (unsigned int i = 0; i < this->size(); ++i) {
	    if (m_thread_vec_[i].joinable())
            m_thread_vec_[i].join();
    }
    
    return BCore::ReturnCode::BSuccess;
}

int BAbstractThreadPool::join(unsigned int _thread_num)
{
    if(_thread_num > this->size())
    {
#ifdef _B_DEBUG_
        B_PRINT_DEBUG("BAbstractThreadPool::join - Thread number "<<_thread_num<<" doesn't exist.")
#endif
        return BCore::ReturnCode::BThreadNotExists;
    }else{
        if (m_thread_vec_[_thread_num].joinable())
            m_thread_vec_[_thread_num].join();
    }    
        
    return BCore::ReturnCode::BSuccess;
}

int BAbstractThreadPool::detach()
{
	for (unsigned int i = 0; i < this->size(); ++i) {
	    if (m_thread_vec_[i].joinable())
            m_thread_vec_[i].detach();
    }
    
    return BCore::ReturnCode::BSuccess;
}

int BAbstractThreadPool::detach(unsigned int _thread_num)
{
    if(_thread_num > this->size())
    {
#ifdef _B_DEBUG_
        B_PRINT_DEBUG("BAbstractThreadPool::join - Thread number "<<_thread_num<<" doesn't exist.")
#endif
        return BCore::ReturnCode::BThreadNotExists;
    }else {
        if (m_thread_vec_[_thread_num].joinable())
            m_thread_vec_[_thread_num].detach();
    }
        
    return BCore::ReturnCode::BSuccess;
}

void BAbstractThreadPool::wait()
{
    std::unique_lock<std::mutex> lock(m_start_mutex_);
    m_start_condition_.wait(lock);
}

void BAbstractThreadPool::setStatus(BThreadPoolStatus _status)
{
	m_pool_status_ = static_cast<int>(_status);
}

int BAbstractThreadPool::status() const
{
	return m_pool_status_;
}

int BAbstractThreadPool::startOneTask()
{
    m_start_condition_.notify_one();
    
    return BCore::ReturnCode::BSuccess;
}

int BAbstractThreadPool::startAllTasks()
{
    m_start_condition_.notify_all();
    
    return BCore::ReturnCode::BSuccess;
}

int BAbstractThreadPool::kill()
{
    //TODO Set timeout for zombie threads.
	this->setStatus(BThreadPoolStatus::ThreadPoolStop);
	/* Notify all threads to get the status flag, then thread will exit.*/
	this->startAllTasks();
	/*Delete all threads.*/
	m_thread_vec_.clear();
	/* Wait until all threads exit. */
	microseconds _ms_time(500);
	while(size() != 0)
	{
		this_thread::sleep_for(_ms_time);
	}
	
    return BCore::ReturnCode::BSuccess;
}

int BAbstractThreadPool::pushTask(BAbstractTask* _task_buffer)
{
    lock_guard<std::mutex> guard(m_task_mutex_);
    
    BAbstractTask* _abstract_task = static_cast<BAbstractTask*>(_task_buffer);
    int _priority = _abstract_task->priority();
    m_priority_task_queue[_priority - 1].push(_task_buffer);
    m_task_bitmap[_priority - 1] = true;
    
    if(m_priority_task_queue[m_priority_state - 1].empty() && _priority < m_priority_state.load())
    {
        m_task_bitmap[m_priority_state - 1] = false;
        bool _has_task = false;
        
        for(int i= m_priority_state - 1;i > 0;i--)
        {
            if(m_task_bitmap[i-1])
            {
                _has_task = true;
                m_priority_state = i;
                break;
            }
        }
        
        // No task in task queue. Reset m_priority_state to default value.
        if(!_has_task)
            m_priority_state = (PriorityNum+1)/2;
    } else
        m_priority_state = _priority > m_priority_state.load()?_priority:m_priority_state.load();
    
    return BCore::ReturnCode::BSuccess;
}

BAbstractTask* BAbstractThreadPool::getTask()
{
    lock_guard<std::mutex> guard(m_task_mutex_);
    
    std::chrono::milliseconds _ms(10);
    std::this_thread::sleep_for(_ms);
    
    bool _has_task = false;
    
    for(int i=0;i < PriorityNum;i++)
    {
        _has_task |= m_task_bitmap[i];
    }
    
    if(_has_task)
    {
        BAbstractTask* _resultTask = m_priority_task_queue[m_priority_state - 1].front();
        m_priority_task_queue[m_priority_state - 1].pop();
        
        // Current priority queue is empty, change the priority state.
        if(m_priority_task_queue[m_priority_state - 1].empty())
        {
            m_task_bitmap[m_priority_state - 1] = false;
            _has_task = false;
            
            for(int i= m_priority_state - 1;i > 0;i--)
            {
                if(m_task_bitmap[i-1])
                {
                    _has_task = true;
                    m_priority_state = i;
                    break;
                }
            }
            
            // No task in task queue. Reset m_priority_state to default value.
            if(!_has_task)
                m_priority_state = (PriorityNum+1)/2;
        }
        
        return _resultTask;
    }else{
        return nullptr;
    }
}

int BAbstractThreadPool::taskQueueSize()
{
	lock_guard<std::mutex> guard(m_task_mutex_);
	
	int _size = 0;
	
	for(int i=0;i < PriorityNum; i++)
	{
	    _size += m_priority_task_queue[i].size();
	}
	
	return _size;
}

int BAbstractThreadPool::sendMessage(int _queue_num, void* _message_buffer)
{
	m_message_mutex_.lock();

	if(m_message_queue_map_.find(_queue_num) == m_message_queue_map_.end())
	{
		m_message_queue_map_[_queue_num];
		m_message_mutex_map_[_queue_num];
		m_message_cond_map_[_queue_num];
		m_message_mutex_.unlock();
	} else {
		m_message_mutex_.unlock();
	}
	
	m_message_mutex_map_[_queue_num].lock();
	m_message_queue_map_[_queue_num].push(_message_buffer);
	m_message_mutex_map_[_queue_num].unlock();
	m_message_cond_map_[_queue_num].notify_all();
	
	return BCore::ReturnCode::BSuccess;
}

void* BAbstractThreadPool::message(int _queue_num)
{
	m_message_mutex_.lock();
	
	if(m_message_queue_map_.find(_queue_num) == m_message_queue_map_.end())
	{
		m_message_queue_map_[_queue_num];
		m_message_mutex_map_[_queue_num];
		m_message_cond_map_[_queue_num];
		m_message_mutex_.unlock();
	} else {
		m_message_mutex_.unlock();
	}
	
	if(m_message_queue_map_[_queue_num].empty()){
		unique_lock<mutex> lock(m_message_mutex_map_[_queue_num]);
		m_message_cond_map_[_queue_num].wait(lock);
		
		if(m_message_queue_map_[_queue_num].empty())
		{
			m_message_mutex_map_[_queue_num].unlock();
		    return nullptr;
	    } else {
		    void* _result_message = m_message_queue_map_[_queue_num].front();
		    m_message_queue_map_[_queue_num].pop();
		    
		    m_message_mutex_map_[_queue_num].unlock();
		    return _result_message;
		}
	} else {
		m_message_mutex_map_[_queue_num].lock();
		
		void* _result_message = m_message_queue_map_[_queue_num].front();
	    m_message_queue_map_[_queue_num].pop();
	    
		m_message_mutex_map_[_queue_num].unlock();
	    return _result_message;
	}
}

void BAbstractThreadPool::m_threadFunction_(void* _buffer)
{
    //Do nothing.
}

};
