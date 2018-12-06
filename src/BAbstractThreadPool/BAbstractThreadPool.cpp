/*
 * @Author      : Ball Chang
 * @File        : BAbstractThreadPool.cpp
 * @Date        : 2018-12-2
*/

#include "BAbstractThreadPool.h"

namespace BThreadPack{

BAbstractThreadPool::BAbstractThreadPool(unsigned int _threadCap,
    BAbstractThreadPool::BThreadControlMode _mode)
    :_threadCap_(_threadCap)
{
}

BAbstractThreadPool::~BAbstractThreadPool()
{
    this->kill();
}

unsigned int BAbstractThreadPool::capacity()
{
    return this->_threadCap_;
}

unsigned int BAbstractThreadPool::size()
{
    return this->_threadVec_.size();
}

long long BAbstractThreadPool::addThread(thread _newThread)
{
    if(this->capacity() == this->size())
    {
        return B_THREAD_POOL_IS_FULL;
    }
    else{
        this->_threadVec_.push_back(std::move(_newThread));
        return this->size();
    }
    return B_SUCCESS;
}

long long BAbstractThreadPool::removeThread(unsigned int _threadNum)
{
	if(_threadNum > this->size())
    {
        return B_THREAAD_NOT_EXISTS;
    }
    else{
        this->_threadVec_.erase(this->_threadVec_.begin() + _threadNum);
        return this->size();
    }
    return B_SUCCESS;
}

int BAbstractThreadPool::_init_(BAbstractThreadPool* _this)
{
	this->setStatus(BThreadPoolStatus::ThreadPoolRunning);
	    
    for (unsigned int i = 0; i < this->capacity(); ++i) {
        this->addThread(thread(BAbstractThreadPool::_threadFunction_, ref(_this)));
    }
    
    return B_SUCCESS;
}

void BAbstractThreadPool::join()
{
	for (unsigned int i = 0; i < this->size(); ++i) {
        this->_threadVec_[i].join();
    }
}

void BAbstractThreadPool::join(unsigned int _threadNum)
{
        this->_threadVec_[_threadNum].join();
}

void BAbstractThreadPool::detach()
{
	for (unsigned int i = 0; i < this->capacity(); ++i) {
        this->_threadVec_[i].detach();
    }
}

void BAbstractThreadPool::detach(unsigned int _threadNum)
{
        this->_threadVec_[_threadNum].detach();
}

void BAbstractThreadPool::wait()
{
    std::unique_lock<std::mutex> lock(this->_startMut_);
    this->_startCond_.wait(lock);
}

void BAbstractThreadPool::setStatus(BThreadPoolStatus _status)
{
	this->_poolStatus_ = (int)_status;
}

int BAbstractThreadPool::status()
{
	return this->_poolStatus_;
}

int BAbstractThreadPool::startOneTask()
{
    this->_startCond_.notify_one();
    
    return B_SUCCESS;
}

int BAbstractThreadPool::startAllTasks()
{
    this->_startCond_.notify_all();
    
    return B_SUCCESS;
}

int BAbstractThreadPool::kill()
{
	this->setStatus(BThreadPoolStatus::ThreadPoolStop);
	/* Notify all threads to get the status flag, then thread will exit.*/
	this->startAllTasks();
	/*Delete all threads.*/
	this->_threadVec_.clear();
	
    return B_SUCCESS;
}

int BAbstractThreadPool::addTask(void* _taskBuffer)
{
    lock_guard<std::mutex> guard(_taskMut_);
    this->_taskQueue_.push(_taskBuffer);
    
    return B_SUCCESS;
}

void* BAbstractThreadPool::getTask()
{
    lock_guard<std::mutex> guard(_taskMut_);
    
    if(!this->_taskQueue_.size())
        return nullptr;
    else{
        void* _resultTask = this->_taskQueue_.front();
        this->_taskQueue_.pop();
        
        return _resultTask;
    }
}

void BAbstractThreadPool::_threadFunction_(void* _buffer)
{
    //Do nothing.
}

};
