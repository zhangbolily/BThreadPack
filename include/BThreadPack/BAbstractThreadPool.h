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

#ifndef INCLUDE_BTHREADPACK_BABSTRACTTHREADPOOL_H_
#define INCLUDE_BTHREADPACK_BABSTRACTTHREADPOOL_H_

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

extern const int32 PriorityNum;
class BAbstractThreadPoolPrivate;

namespace BThreadPack {

using std::chrono::microseconds;

class BAbstractThreadPool: private NoneCopy{
 public:
    enum BThreadPoolStatus {
        ThreadPoolError = -1,
        ThreadPoolStop = 0,
        ThreadPoolRunning = 1
    };

    enum BThreadControlMode {
        /* In this mode, thread capacity is fixed. */
        FixedThreadCapacity = 0,
        /* In this mode, thread capacity is dynamic controled.
         * If application doesn't specify the thread capacity, BAbstractThreadPool will caculate
         * a proper thread capacity baseed on the system hardware.
        */
        DynamicThreadCapacity = 1,
    };

    explicit BAbstractThreadPool(uint _thread_cap);
    BAbstractThreadPool(uint _thread_cap,
                        BAbstractThreadPool::BThreadControlMode _mode);

    /* @~BAbstractThreadPool() - Destructor */
    ~BAbstractThreadPool();

    int32 taskQueueSize();
    int32 sendMessage(int32 _queue_num, void* _message_buffer);
    int32 status() const;
    int32 kill();
    int32 mode() const;
    uint capacity() const;
    uint size() const;
    void setCapacity(uint _capacity);
    void setStatus(BThreadPoolStatus _status);
    void pushTask(BAbstractTask* _task_ptr);
    void pushGroupTask(BGroupTask* _task_ptr);
    void* message(int32 _queue_num);
    BAbstractTask* getFinishedTask();

    virtual uint resize(uint _size) = 0;

 protected:
    BAbstractThreadPoolPrivate* m_private_ptr;
};

}  // namespace BThreadPack

#endif  // INCLUDE_BTHREADPACK_BABSTRACTTHREADPOOL_H_
