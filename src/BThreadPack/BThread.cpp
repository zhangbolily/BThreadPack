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
 * @File        : BThread.cpp
 * @Date        : 2019-3-25
*/

#include "BThreadPack/BThread.h"
#include "BThreadPack/private/BThreadPrivate.h"

namespace BThreadPack {

BThread::~BThread() {
    if (m_private_ptr != nullptr)
        delete m_private_ptr;
}

bool BThread::joinable() {
    return m_private_ptr->m_thread_handle.joinable();
}

bool BThread::isExit() {
    return m_private_ptr->m_thread_info.exited;
}

bool BThread::isRunning() {
    return m_private_ptr->m_thread_info.running;
}

int32 BThread::join() {
    if (joinable()) {
        m_private_ptr->m_thread_handle.join();
        return BSuccess;
    } else {
        return BError;
    }
}

int32 BThread::detach() {
    if (joinable()) {
        m_private_ptr->m_thread_handle.detach();
        return BSuccess;
    } else {
        return BError;
    }
}

int32 BThread::priority() {
    // TODO(Ball Chang) :Get thread priority on any platform.
    return 0;
}

int32 BThread::setAffinity(int32 cpu_num) {
#if defined(LINUX)
    return 0;
#elif defined(WIN32)
#endif
}

std::thread::id BThread::id() {
    return m_private_ptr->m_thread_handle.get_id();
}

std::thread::native_handle_type BThread::nativeHandle() {
    return m_private_ptr->m_thread_handle.native_handle();
}

#ifdef LINUX
pid_t BThread::tid() {
    pid_t null_obj;
    return null_obj;
}
#endif

}  // namespace BThreadPack
