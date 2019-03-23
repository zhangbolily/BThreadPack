/* MIT License
* 
* Copyright (c) 2018 Ball Chang
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

/*
 * @Author      : Ball Chang
 * @File        : BAbstractThreadPool.h
 * @Date        : 2018-12-2
*/

#ifndef _BABSTRACT_THREAD_POOL_H_
#define _BABSTRACT_THREAD_POOL_H_

#ifdef WIN32
#include "windows.h"
#endif

#include <atomic>
#include <condition_variable>
#include <chrono>
#include <functional>
#include <map>
#include <queue>
#include <thread>
#include <vector>

#include "BThreadPack/BThreadPack.h"
#include "BThreadPack/BAbstractClass.h"
#include "BThreadPack/BAbstractTask.h"
#include "BThreadPack/BGroupTask.h"

using namespace std;
using namespace std::chrono;

extern const int PriorityNum;
class BAbstractThreadPoolPrivate;

namespace BThreadPack {

class BAbstractThreadPool: private NoneCopy{

public:
	enum BThreadPoolStatus{
        ThreadPoolError = -1,
        ThreadPoolStop = 0,
        ThreadPoolRunning = 1
    };
    
    enum BThreadControlMode{
        /* In this mode, thread capacity is fixed. */
        FixedThreadCapacity = 0,
        /* In this mode, thread capacity is dynamic controled.
         * If application doesn't specify the thread capacity, BAbstractThreadPool will caculate
         * a proper thread capacity baseed on the system hardware.
        */
        DynamicThreadCapacity = 1,
    };

    BAbstractThreadPool(unsigned int _thread_cap);
    BAbstractThreadPool(unsigned int _thread_cap,
                        BAbstractThreadPool::BThreadControlMode _mode);
    
    /* @~BAbstractThreadPool() - Destructor */
    ~BAbstractThreadPool();
    
    long long addThread(thread _new_thread);
    long long setAffinity();
    long long setAffinity(unsigned int _thread_num);
    long long removeThread(unsigned int _thread_num);
    bool isRemove();
    bool isThreadExit(std::thread::id _tid);
    int taskQueueSize();
    int sendMessage(int _queue_num, void* _message_buffer);
    int status() const;
    int startOneTask();
    int startAllTasks();
    int kill();
    int mode() const;
    int detach();
    int detach(unsigned int _thread_num);
    int join();
    int join(unsigned int _thread_num);
    unsigned int capacity() const;
    unsigned int size() const;
    void wait();
    void setCapacity(unsigned int _capacity);
    void setStatus(BThreadPoolStatus _status);
    void pushTask(BAbstractTask* _task_ptr);
    void pushGroupTask(BGroupTask* _task_ptr);
    void threadExit(std::thread::id _tid);
    void* message(int _queue_num);
    BAbstractTask* getTask();
    
    virtual unsigned int resize(unsigned int _size)=0;

protected:
    BAbstractThreadPoolPrivate* m_private_ptr;
};

};

#endif
