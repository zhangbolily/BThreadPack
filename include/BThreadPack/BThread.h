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
 * @File        : BThread.h
 * @Date        : 2019-3-25
*/

#ifndef INCLUDE_BTHREADPACK_BTHREAD_H_
#define INCLUDE_BTHREADPACK_BTHREAD_H_

#include <utility>
#include <thread>
#include <atomic>
#include <algorithm>

#ifdef LINUX
#include <sys/types.h>
#endif

#include "BThreadPack/BThreadPack.h"
#include "BThreadPack/BAbstractClass.h"

namespace BThreadPack {

using std::thread;
using std::forward;
using std::atomic_bool;
using std::atomic_int32_t;
using BCore::ReturnCode;

class BThreadPrivate;
class BAbstractThreadPool;


class BThreadInfo {
 public:
    BThreadInfo();
    BThreadInfo(const BThreadInfo& thread_info);

    BThreadInfo& operator=(const BThreadInfo& thread_info);

    void setThreadPoolHandle(BAbstractThreadPool* thread_pool_handle);
    void exit(int32 return_code = 0);
    void running();
    bool isExit();
    bool isRunning();
    BAbstractThreadPool* threadPoolHandle();
    std::thread::id id();

 protected:
    std::atomic_bool is_exited;
    std::atomic_bool is_running;
    std::atomic_int32_t stackSize;
    std::atomic_int32_t returnCode;
    std::thread::id     m_id;
    BAbstractThreadPool* m_thread_pool_handle;
};

class BThread {
 public:
    BThread();
    BThread(const BThread& _bthread) noexcept;
    BThread(BThread&& _bthread) noexcept;
    ~BThread();

    BThread& operator=(BThread&& _bthread) noexcept;

    template< class Function >
    void start(Function&& f, const BThreadInfo& thread_info) {
        if (m_thread_info_ptr != nullptr)
            delete m_thread_info_ptr;
        else
            m_thread_info_ptr = new BThreadInfo(thread_info);
        m_thread_handle = new std::thread(std::forward<Function>(f),
                                  std::ref(*m_thread_info_ptr));
#ifdef _B_DEBUG_
        B_PRINT_DEBUG("BThread::start - "
        "Create a new thread # " << id())
#endif
    }

    bool joinable();
    bool isExit();
    bool isRunning();
    int32 join();
    int32 detach();
    int32 priority();
    int32 setAffinity(int32 cpu_num);
#ifdef LINUX
    pid_t tid();
#endif
    std::thread::id id();
    std::thread::native_handle_type nativeHandle();

 protected:
    BThreadPrivate* m_private_ptr;
    BThreadInfo* m_thread_info_ptr;
    std::thread* m_thread_handle;
};

}   // namespace BThreadPack


#endif  // INCLUDE_BTHREADPACK_BTHREAD_H_
