/*
 * @Author      : Ball Chang
 * @File        : BGeneralThreadPool.h
 * @Date        : 2018-12-3
*/

#ifndef _BGENERAL_THREAD_POOL_H_
#define _BGENERAL_THREAD_POOL_H_

#include "BAbstractThreadPool.h"
#include "BGeneralTask.h"

using namespace std;

namespace BThreadPack {

class BGeneralThreadPool: public BAbstractThreadPool{

public:
    /* @BAbstractThreadPool() - Constructor
     * @_thread_cap - How many threads can be created in this pool.
    */
    BGeneralThreadPool(unsigned int _thread_cap);
    
    /* @BAbstractThreadPool() - Constructor
     * @_thread_cap - How many threads can be created in this pool.
     * @_mode - This thread pool will work in which mode.Work mode is defined in BThreadControlMode.
    */
    BGeneralThreadPool(unsigned int _thread_cap,
                        BAbstractThreadPool::BThreadControlMode _mode);
    
    /* @~BAbstractThreadPool() - Destructor */
    ~BGeneralThreadPool();
    
private:
	/* @_init_ - This function will initialize all threads.
     * @_this - Pass a fake this pointer into thread.
     * All threads will use this pointer to call public member in this class.
     * Mostly for task management purpose.
     * @return - return B_SUCCESS if success
    */
    virtual int m_init_(BGeneralThreadPool* _this);
    
    static void m_threadFunction_(BGeneralThreadPool* _this);
};

};

#endif
