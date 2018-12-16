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

void BGeneralThreadPool::m_threadFunction_(BGeneralThreadPool* _this)
{
    while(1)
    {
        BGeneralTask* p_general_task = (BGeneralTask*)_this->getTask();
        
        if(p_general_task == nullptr)
        {
            _this->wait();
            
            if(_this->status() == BAbstractThreadPool::BThreadPoolStatus::ThreadPoolStop)
            {
            	return;
            }else
            	continue;
        }
        
        p_general_task->setStatus(BGeneralTask::BTaskStatus::TaskProcessing);
        
        int _retcode = p_general_task->process();
            
        if(_retcode == B_ERROR)
        {
        	p_general_task->setStatus(BGeneralTask::BTaskStatus::TaskFailed);
        }else{
        	p_general_task->setStatus(BGeneralTask::BTaskStatus::TaskFinished);
        }
    }
}

};
