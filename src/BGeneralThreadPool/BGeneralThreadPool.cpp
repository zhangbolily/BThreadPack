/*
 * @Author      : Ball Chang
 * @File        : BAbstractThreadPool.cpp
 * @Date        : 2018-12-2
*/

#include "BGeneralThreadPool.h"

namespace BThreadPack{

BGeneralThreadPool::BGeneralThreadPool(int _threadNum)
    :BAbstractThreadPool(_threadNum)
{
	this->_initThreads_(this);
}

BGeneralThreadPool::~BGeneralThreadPool()
{
}

int BGeneralThreadPool::_initThreads_(BGeneralThreadPool* _this)
{
    for (unsigned int i = 0; i < this->getThreadCap(); ++i) {
        if(this->addThread(thread(BGeneralThreadPool::_threadFunction_, _this)) == B_THREAD_POOL_IS_FULL)
            return B_THREAD_POOL_IS_FULL;
    }
    
    this->detachThreads();
    
    return B_SUCCESS;
}

void BGeneralThreadPool::_threadFunction_(BGeneralThreadPool* _this)
{
    while(1)
    {
        BGeneralTask* _pGeneralTask = (BGeneralTask*)_this->getTask();
        
        if(_pGeneralTask == nullptr)
        {
            _this->waitCond();
            
            if(_this->getThreadPoolStatus() == BAbstractThreadPool::BThreadPoolStatus::ThreadPoolStop)
            {
            	return;
            }else
            	continue;
        }
            
        if(_pGeneralTask->processTask() == B_ERROR)
        {
        }
    }
}

};
