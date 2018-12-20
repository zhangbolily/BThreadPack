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
            unsigned int _tempCapcity = thread::hardware_concurrency() * 2;
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
        return B_THREAD_POOL_IS_FULL;
    }
    else{
        m_thread_vec_.push_back(std::move(_new_thread));
        return this->size();
    }
    return B_SUCCESS;
}

long long BAbstractThreadPool::removeThread(unsigned int _thread_num)
{
	if(_thread_num > this->size())
    {
#ifdef _B_DEBUG_
        B_PRINT_DEBUG("BAbstractThreadPool::removeThread - Thread number "<<_thread_num<<" doesn't exist.")
#endif
        return B_THREAD_NOT_EXISTS;
    }
    else{
        m_thread_vec_.erase(m_thread_vec_.begin() + _thread_num);
        return this->size();
    }
    return B_SUCCESS;
}

int BAbstractThreadPool::_init_(BAbstractThreadPool* _this)
{
	this->setStatus(BThreadPoolStatus::ThreadPoolRunning);
	    
    for (unsigned int i = 0; i < this->capacity(); ++i) {
        this->addThread(thread(BAbstractThreadPool::m_threadFunction_, ref(_this)));
    }
    
    return B_SUCCESS;
}

int BAbstractThreadPool::join()
{
	for (unsigned int i = 0; i < this->size(); ++i) {
        m_thread_vec_[i].join();
    }
    
    return B_SUCCESS;
}

int BAbstractThreadPool::join(unsigned int _thread_num)
{
    if(_thread_num > this->size())
    {
#ifdef _B_DEBUG_
        B_PRINT_DEBUG("BAbstractThreadPool::join - Thread number "<<_thread_num<<" doesn't exist.")
#endif
        return B_THREAD_NOT_EXISTS;
    }else
        m_thread_vec_[_thread_num].join();
        
    return B_SUCCESS;
}

int BAbstractThreadPool::detach()
{
	for (unsigned int i = 0; i < this->size(); ++i) {
        m_thread_vec_[i].detach();
    }
    
    return B_SUCCESS;
}

int BAbstractThreadPool::detach(unsigned int _thread_num)
{
    if(_thread_num > this->size())
    {
#ifdef _B_DEBUG_
        B_PRINT_DEBUG("BAbstractThreadPool::join - Thread number "<<_thread_num<<" doesn't exist.")
#endif
        return B_THREAD_NOT_EXISTS;
    }else
        m_thread_vec_[_thread_num].detach();
        
    return B_SUCCESS;
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
    
    return B_SUCCESS;
}

int BAbstractThreadPool::startAllTasks()
{
    m_start_condition_.notify_all();
    
    return B_SUCCESS;
}

int BAbstractThreadPool::kill()
{
	this->setStatus(BThreadPoolStatus::ThreadPoolStop);
	/* Notify all threads to get the status flag, then thread will exit.*/
	this->startAllTasks();
	/*Delete all threads.*/
	m_thread_vec_.clear();
	
    return B_SUCCESS;
}

int BAbstractThreadPool::addTask(void* _task_buffer)
{
    lock_guard<std::mutex> guard(m_task_mutex_);
    m_task_queue_.push(_task_buffer);
    
    return B_SUCCESS;
}

void* BAbstractThreadPool::getTask()
{
    lock_guard<std::mutex> guard(m_task_mutex_);
    
    if(!m_task_queue_.size())
        return nullptr;
    else{
        void* _resultTask = m_task_queue_.front();
        m_task_queue_.pop();
        
        return _resultTask;
    }
}

void BAbstractThreadPool::m_threadFunction_(void* _buffer)
{
    //Do nothing.
}

};
