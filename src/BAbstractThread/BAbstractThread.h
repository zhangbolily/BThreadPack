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
    BAbstractThread(bool _detach);
    ~BAbstractThread();

    /* @kill - Kill this thread and release all resources.
     * Don't need any parameters.
    */
    void kill();

private:
    atomic_int _pid_;
    atomic_bool _detach_;

    virtual void ThreadFunction();

};

};

#endif
