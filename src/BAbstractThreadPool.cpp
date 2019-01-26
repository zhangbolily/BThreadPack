/*
 * @Author      : Ball Chang
 * @File        : BAbstractThreadPool.cpp
 * @Date        : 2018-12-2
*/

#include "BAbstractThreadPool.h"

namespace BThreadPack{

BAbstractThreadPool::BAbstractThreadPool(unsigned int _thread_cap)
    :m_pool_mode_(BAbstractThreadPool::BThreadControlMode::FixedThreadCapacity)
{
    this->setCapacity(_thread_cap);
}

BAbstractThreadPool::BAbstractThreadPool(unsigned int _thread_cap,
    BAbstractThreadPool::BThreadControlMode _mode)
    :m_pool_mode_(_mode)
{
    this->setCapacity(_thread_cap);
}

BAbstractThreadPool::~BAbstractThreadPool()
{
    this->kill();
}

int BAbstractThreadPool::mode()
{
    return m_pool_mode_;
}

unsigned int BAbstractThreadPool::capacity()
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

unsigned int BAbstractThreadPool::size()
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
        return ReturnCode::BThreadPoolFull;
    }
    else{
        m_thread_vec_.push_back(std::move(_new_thread));
        return this->size();
    }
    return ReturnCode::BSuccess;
}

long long BAbstractThreadPool::removeThread(unsigned int _thread_num)
{
	if(_thread_num > this->size())
    {
#ifdef _B_DEBUG_
        B_PRINT_DEBUG("BAbstractThreadPool::removeThread - Thread number "<<_thread_num<<" doesn't exist.")
#endif
        return ReturnCode::BThreadNotExists;
    }
    else{
        m_thread_vec_.erase(m_thread_vec_.begin() + _thread_num);
        return this->size();
    }
    return ReturnCode::BSuccess;
}

int BAbstractThreadPool::_init_(BAbstractThreadPool* _this)
{
	this->setStatus(BThreadPoolStatus::ThreadPoolRunning);
	    
    for (unsigned int i = 0; i < this->capacity(); ++i) {
        this->addThread(thread(BAbstractThreadPool::m_threadFunction_, ref(_this)));
    }
    
    return ReturnCode::BSuccess;
}

int BAbstractThreadPool::join()
{
	for (unsigned int i = 0; i < this->size(); ++i) {
        m_thread_vec_[i].join();
    }
    
    return ReturnCode::BSuccess;
}

int BAbstractThreadPool::join(unsigned int _thread_num)
{
    if(_thread_num > this->size())
    {
#ifdef _B_DEBUG_
        B_PRINT_DEBUG("BAbstractThreadPool::join - Thread number "<<_thread_num<<" doesn't exist.")
#endif
        return ReturnCode::BThreadNotExists;
    }else
        m_thread_vec_[_thread_num].join();
        
    return ReturnCode::BSuccess;
}

int BAbstractThreadPool::detach()
{
	for (unsigned int i = 0; i < this->size(); ++i) {
        m_thread_vec_[i].detach();
    }
    
    return ReturnCode::BSuccess;
}

int BAbstractThreadPool::detach(unsigned int _thread_num)
{
    if(_thread_num > this->size())
    {
#ifdef _B_DEBUG_
        B_PRINT_DEBUG("BAbstractThreadPool::join - Thread number "<<_thread_num<<" doesn't exist.")
#endif
        return ReturnCode::BThreadNotExists;
    }else
        m_thread_vec_[_thread_num].detach();
        
    return ReturnCode::BSuccess;
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

int BAbstractThreadPool::status()
{
	return m_pool_status_;
}

int BAbstractThreadPool::startOneTask()
{
    m_start_condition_.notify_one();
    
    return ReturnCode::BSuccess;
}

int BAbstractThreadPool::startAllTasks()
{
    m_start_condition_.notify_all();
    
    return ReturnCode::BSuccess;
}

int BAbstractThreadPool::kill()
{
    //TODO Set timeout for zombie threads.
	this->setStatus(BThreadPoolStatus::ThreadPoolStop);
	/* Notify all threads to get the status flag, then thread will exit.*/
	this->startAllTasks();
	/*Delete all threads.*/
	m_thread_vec_.clear();
	/* Wait all threads exit. */
	microseconds _us_time(500);
	while(size() != 0)
	{
		this_thread::sleep_for(_us_time);
	}
	
    return ReturnCode::BSuccess;
}

int BAbstractThreadPool::addTask(void* _task_buffer)
{
    lock_guard<std::mutex> guard(m_task_mutex_);
    m_task_queue_.push(_task_buffer);
    
    return ReturnCode::BSuccess;
}

void* BAbstractThreadPool::getTask()
{
    lock_guard<std::mutex> guard(m_task_mutex_);
    
    if(m_task_queue_.empty())
        return nullptr;
    else{
        void* _resultTask = m_task_queue_.front();
        m_task_queue_.pop();
        
        return _resultTask;
    }
}

int BAbstractThreadPool::taskQueueSize()
{
	lock_guard<std::mutex> guard(m_task_mutex_);
	return m_task_queue_.size();
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
	
	return ReturnCode::BSuccess;
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
