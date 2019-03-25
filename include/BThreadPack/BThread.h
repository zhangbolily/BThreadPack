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

#ifdef LINUX
#include <sys/types.h>
#endif

#include "BThreadPack/BThreadPack.h"
#include "BThreadPack/BAbstractClass.h"

namespace BThreadPack {

using std::thread;
using std::forward;
using BCore::ReturnCode;

class BThreadPrivate;
class BAbstractThreadPool;

class BThread : private NoneCopy {
 public:
    class BThreadInfo {
     public:
        bool exited = false;
        bool running = false;
        uint32 stackSize = 0;
        int32 returnCode = 0;
        BAbstractThreadPool* m_thread_pool_handle = nullptr;

        void exit();
    };

    BThread() noexcept = default;

    template < class Function >
    explicit BThread(Function&& f, const BThreadInfo& thread_info)
            : m_private_ptr(new BThreadPrivate(std::forward<Function>(f),
                            thread_info)) {
    }

    template < class Function >
    explicit BThread(bool is_detach, Function&& f, const BThreadInfo& thread_info)
            : m_private_ptr(new BThreadPrivate(std::forward<Function>(f),
                            thread_info)) {
        if (is_detach)
            detach();
    }

    ~BThread();

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
};
}   // namespace BThreadPack


#endif  // INCLUDE_BTHREADPACK_BTHREAD_H_
