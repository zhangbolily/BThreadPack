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
     * @_thread_num - How many thread you want to create.
    */
    BGeneralThreadPool(int _threadNum);
    
    /* @~BAbstractThreadPool() - Destructor
     * Don't need any parameter
    */
    ~BGeneralThreadPool();
    
    /* @_initThreads_ - This function will initialize all threads.
     * @_this - Pass a fake this pointer into thread.
     * All threads will use this pointer to call public member in this class.
     * Mostly for task management purpose.
     * @return - return 0 if success
    */
    int initThreads(BGeneralThreadPool* _this);
    
private:
    static void _threadFunction_(BGeneralThreadPool* _this);
};

};

#endif
