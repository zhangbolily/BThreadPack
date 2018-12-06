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
     * @_threadCap - How many threads can be created in this pool.
    */
    BGeneralThreadPool(unsigned int _threadCap);
    
    /* @~BAbstractThreadPool() - Destructor
     * Don't need any parameter
    */
    ~BGeneralThreadPool();
    
private:
	/* @_initThreads_ - This function will initialize all threads.
     * @_this - Pass a fake this pointer into thread.
     * All threads will use this pointer to call public member in this class.
     * Mostly for task management purpose.
     * @return - return 0 if success
    */
    int _initThreads_(BGeneralThreadPool* _this);
    
    static void _threadFunction_(BGeneralThreadPool* _this);
};

};

#endif
