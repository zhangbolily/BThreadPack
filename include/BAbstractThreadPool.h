/*
 * @Author      : Ball Chang
 * @File        : BAbstractThreadPool.h
 * @Date        : 2018-12-2
*/

#ifndef _BABSTRACT_THREAD_POOL_H_
#define _BABSTRACT_THREAD_POOL_H_

#include <thread>
#include <atomic>
#include "BThreadPack.h"

using namespace std;

namespace BThreadPack {

class BAbstractThreadPool{

public:
    /* @BAbstractThreadPool() - Constructor
     * @_thread_num - How many thread you want to create.
    */
    BAbstractThreadPool(int _thread_num);
    
    /* @~BAbstractThreadPool() - Destructor
     * Don't need any parameter
    */
    ~BAbstractThreadPool();

    /* @kill - Kill all threads and release all resources.
     * Don't need any parameters.
    */
    void kill();

private:
    static void ThreadFunction(void * _buffer);

};

};

#endif
