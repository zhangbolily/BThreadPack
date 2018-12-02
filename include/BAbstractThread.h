/*
 * @Author      : Ball Chang
 * @File        : BAbstractThread.h
 * @Date        : 2018-11-12
*/


#ifndef _BABSTRACT_THREAD_H_
#define _BABSTRACT_THREAD_H_

#include <thread>
#include <atomic>

using namespace std;

namespace BThreadPack {

class BAbstractThread{

public:
    /* @BAbstractThread() - Constructor
     * Don't need any parameter
    */
    BAbstractThread(bool _detach);
    
    /* @~BAbstractThread() - Destructor
     * Don't need any parameter
    */
    ~BAbstractThread();

    /* @kill - Kill this thread and release all resources.
     * Don't need any parameters.
    */
    void kill();

private:
    thread::id _pid_;
    atomic_bool _detach_;

    virtual void ThreadFunction(void * _buffer);

};

};

#endif
