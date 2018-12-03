/*
 * @Author      : Ball Chang
 * @File        : BAbstractThreadPool.cpp
 * @Date        : 2018-12-2
*/

#include "BAbstractThreadPool.h"

namespace BThreadPack{

BAbstractThreadPool::BAbstractThreadPool(int _threadNum)
    :_threadNum_(_threadNum)
{
}

BAbstractThreadPool::~BAbstractThreadPool()
{
    //Do nothing.
}

unsigned int BAbstractThreadPool::getThreadCap()
{
    return this->_threadNum_;
}

unsigned int BAbstractThreadPool::getThreadNum()
{
    return this->_threadVec_.size();
}

long long BAbstractThreadPool::addThread(thread _newThread)
{
    if(this->_threadNum_ == this->_threadVec_.size())
    {
        return B_THREAD_POOL_IS_FULL;
    }
    else{
        this->_threadVec_.push_back(std::move(_newThread));
        return this->_threadNum_;
    }
    return 0;
}

int BAbstractThreadPool::initThreads(BAbstractThreadPool* _this)
{    
    for (unsigned int i = 0; i < this->getThreadCap(); ++i) {
        this->addThread(thread(BAbstractThreadPool::_threadFunction_, ref(_this)));
    }
    
    return 0;
}

void BAbstractThreadPool::waitCond()
{
    std::unique_lock<std::mutex> lock(this->_startMut_);
    this->_startCond_.wait(lock);
}

int BAbstractThreadPool::startOneTask()
{
    this->_startCond_.notify_one();
    
    return 0;
}

int BAbstractThreadPool::startAllTask()
{
    this->_startCond_.notify_all();
    
    return 0;
}

int BAbstractThreadPool::kill()
{
    return 0;
}

int BAbstractThreadPool::addTask(void* _taskBuffer)
{
    lock_guard<std::mutex> guard(_taskMut_);
    this->_taskQueue_.push(_taskBuffer);
    
    return 0;
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
