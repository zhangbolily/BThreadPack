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
}

BGeneralThreadPool::~BGeneralThreadPool()
{
}

int BGeneralThreadPool::initThreads(BGeneralThreadPool* _this)
{
    for (unsigned int i = 0; i < this->getThreadCap(); ++i) {
        if(this->addThread(thread(BGeneralThreadPool::_threadFunction_, _this)) == B_THREAD_POOL_IS_FULL)
            return B_THREAD_POOL_IS_FULL;
    }
    
    return B_SUCCESS;
}

void _threadFunction_(BGeneralThreadPool* _this)
{
    while(1)
    {
        BGeneralTask* _pGeneralTask = (BGeneralTask*)_this->getTask();
        
        if(_pGeneralTask == nullptr)
        {
            _this->waitCond();
            continue;
        }
            
        if(_pGeneralTask->processTask() == B_ERROR)
        {
        }
    }
}

};
