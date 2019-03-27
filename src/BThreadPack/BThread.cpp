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

// Star implementation of BThreadInfo

BThreadInfo::BThreadInfo()
        : is_exited (false),
          is_running (false),
          stackSize (0),
          returnCode (0),
          m_thread_pool_handle (nullptr){

}

BThreadInfo::BThreadInfo(const BThreadInfo& thread_info)
        : is_exited (thread_info.is_exited.load()),
          is_running (thread_info.is_running.load()),
          stackSize (thread_info.stackSize.load()),
          returnCode (thread_info.returnCode.load()),
          m_thread_pool_handle (nullptr) {

}

BThreadInfo& BThreadInfo::operator=(const BThreadInfo& thread_info) {
    is_exited.store(thread_info.is_exited);
    is_running.store(thread_info.is_running);
    stackSize.store(thread_info.stackSize);
    returnCode.store(thread_info.returnCode);
    m_thread_pool_handle = thread_info.m_thread_pool_handle;

    return *this;
}

void BThreadInfo::exit(int32 return_code) {
    is_exited = true;
    is_running = false;
    returnCode = return_code;
    return;
}

void BThreadInfo::running() {
    is_exited = false;
    is_running = true;
}

bool BThreadInfo::isExit() {
    return is_exited;
}

bool BThreadInfo::isRunning() {
    return is_running;
}

// End of implementation


// Star implementation of BThread

BThread::BThread()
    : m_private_ptr(new BThreadPrivate) {
}

BThread::~BThread() {
    if (m_private_ptr != nullptr)
        delete m_private_ptr;
}

bool BThread::joinable() {
    return m_thread_handle.joinable();
}

bool BThread::isExit() {
    return m_thread_info.isExit();
}

bool BThread::isRunning() {
    return m_thread_info.isRunning();
}

int32 BThread::join() {
    if (joinable()) {
        m_thread_handle.join();
        return BSuccess;
    } else {
        return BError;
    }
}

int32 BThread::detach() {
    if (joinable()) {
        m_thread_handle.detach();
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
    // TODO(Ball Chang) :Set affinity on any platform.
    return 0;
#elif defined(WIN32)
#endif
}

std::thread::id BThread::id() {
    return m_thread_handle.get_id();
}

std::thread::native_handle_type BThread::nativeHandle() {
    return m_thread_handle.native_handle();
}

#ifdef LINUX
pid_t BThread::tid() {
    pid_t null_obj;
    return null_obj;
}
#endif

// End of implementation

}  // namespace BThreadPack
