/*
 * @Author      : Ball Chang
 * @File        : BAbstractThreadPool.cpp
 * @Date        : 2018-12-2
*/

#include "BGeneralThreadPool.h"

namespace BThreadPack{

BGeneralThreadPool::BGeneralThreadPool(unsigned int _threadCap)
    :BAbstractThreadPool(_threadCap)
{
	this->_initThreads_(this);
}

BGeneralThreadPool::~BGeneralThreadPool()
{
}

int BGeneralThreadPool::_initThreads_(BGeneralThreadPool* _this)
{
	this->setStatus(BThreadPoolStatus::ThreadPoolRunning);
	
    for (unsigned int i = 0; i < this->capacity(); ++i) {
        if(this->addThread(thread(BGeneralThreadPool::_threadFunction_, _this)) == B_THREAD_POOL_IS_FULL)
            return B_THREAD_POOL_IS_FULL;
    }
    
    this->detach();
    
    return B_SUCCESS;
}

void BGeneralThreadPool::_threadFunction_(BGeneralThreadPool* _this)
{
    while(1)
    {
        BGeneralTask* _pGeneralTask = (BGeneralTask*)_this->getTask();
        
        if(_pGeneralTask == nullptr)
        {
            _this->wait();
            
            if(_this->status() == BAbstractThreadPool::BThreadPoolStatus::ThreadPoolStop)
            {
            	return;
            }else
            	continue;
        }
        
        _pGeneralTask->setStatus(BGeneralTask::BTaskStatus::TaskProcessing);
        
        int _retcode = _pGeneralTask->process();
            
        if(_retcode == B_ERROR)
        {
        	_pGeneralTask->setStatus(BGeneralTask::BTaskStatus::TaskFailed);
        }else{
        	_pGeneralTask->setStatus(BGeneralTask::BTaskStatus::TaskFinished);
        }
    }
}

};
