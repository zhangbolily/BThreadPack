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
#include "BThreadPack/BAbstractThreadPool.h"

namespace BThreadPack {

// Star implementation of BThreadInfo
BThreadInfo::BThreadInfo()
        : is_exited(false),
          is_running(false),
          stackSize(0),
          returnCode(0),
          m_thread_pool_handle(nullptr) {
}

BThreadInfo::BThreadInfo(const BThreadInfo& thread_info)
        : is_exited(thread_info.is_exited.load()),
          is_running(thread_info.is_running.load()),
          stackSize(thread_info.stackSize.load()),
          returnCode(thread_info.returnCode.load()),
          m_thread_pool_handle(thread_info.m_thread_pool_handle) {
#ifdef _B_DEBUG_
    B_PRINT_DEBUG("BThreadInfo::BThreadInfo(const BThreadInfo& thread_info)"
                  " - Copy construct function was called.")
#endif
}

BThreadInfo& BThreadInfo::operator=(const BThreadInfo& thread_info) {
    is_exited.store(thread_info.is_exited.load());
    is_running.store(thread_info.is_running.load());
    stackSize.store(thread_info.stackSize.load());
    returnCode.store(thread_info.returnCode.load());
    stackSize.store(thread_info.stackSize);
    m_thread_pool_handle = thread_info.m_thread_pool_handle;
#ifdef _B_DEBUG_
    B_PRINT_DEBUG("BThreadInfo::operator=(const BThreadInfo& thread_info)"
                  " - Copy function was called.")
#endif
    return *this;
}

void BThreadInfo::setThreadPoolHandle(BAbstractThreadPool* thread_pool_handle) {
    m_thread_pool_handle = thread_pool_handle;
}

void BThreadInfo::exit(int32 return_code) {
    is_exited.store(true, std::memory_order_release);
    is_running.store(false, std::memory_order_release);
    returnCode.store(return_code, std::memory_order_release);
    m_id = std::thread::id();
#ifdef _B_DEBUG_
    B_PRINT_DEBUG("BThreadInfo::exit - Exit with return code "
    << returnCode << " and is_exited is " << is_exited
    << " - BThreadInfo address is :" << this)
#endif
    return;
}

void BThreadInfo::running() {
    is_exited.store(false, std::memory_order_release);
    is_running.store(true, std::memory_order_release);
    m_id = std::this_thread::get_id();
#ifdef _B_DEBUG_
    B_PRINT_DEBUG("BThreadInfo::running"
    << " - BThreadInfo address is :" << this)
#endif
    return;
}

bool BThreadInfo::isExit() {
#ifdef _B_DEBUG_
    B_PRINT_DEBUG("BThreadInfo::isExit"
    << " - BThreadInfo address is :" << this
    << " and is_exited is " << is_exited)
#endif
    return is_exited.load(std::memory_order_acquire);
}

bool BThreadInfo::isRunning() {
#ifdef _B_DEBUG_
    B_PRINT_DEBUG("BThreadInfo::isRunning"
    << " - BThreadInfo address is :" << this)
#endif
    return is_running.load(std::memory_order_acquire);
}

BAbstractThreadPool* BThreadInfo::threadPoolHandle() {
    return m_thread_pool_handle;
}

std::thread::id BThreadInfo::id() {
    return m_id;
}

// End of implementation


// Star implementation of BThread

BThread::BThread()
    : m_private_ptr(new BThreadPrivate),
      m_thread_handle(nullptr),
      m_thread_info_ptr(nullptr) {
}

BThread::BThread(const BThread& _bthread) noexcept {
#ifdef _B_DEBUG_
    B_PRINT_DEBUG("BThread::BThread(const BThread&& _bthread)"
                  " - Copy construct function was called.")
#endif

    *m_private_ptr = *_bthread.m_private_ptr;
    m_thread_info_ptr = new BThreadInfo();
    *m_thread_info_ptr = *_bthread.m_thread_info_ptr;
    if (_bthread.m_thread_handle == nullptr) {
        m_thread_handle = nullptr;
    } else {
        m_thread_handle->swap(*_bthread.m_thread_handle);
    }
}

BThread::BThread(BThread&& _bthread) noexcept {
#ifdef _B_DEBUG_
    B_PRINT_DEBUG("BThread::BThread(BThread&& _bthread)"
                  " - Copy construct function was called.")
#endif
    m_private_ptr = _bthread.m_private_ptr;
    m_thread_info_ptr = _bthread.m_thread_info_ptr;
    if (_bthread.m_thread_handle == nullptr) {
        m_thread_handle = nullptr;
    } else {
        m_thread_handle = new thread(std::move(*(_bthread.m_thread_handle)));
    }

    _bthread.m_thread_info_ptr = nullptr;
    _bthread.m_thread_handle = nullptr;
    _bthread.m_private_ptr = nullptr;
}

BThread::~BThread() {
#ifdef _B_DEBUG_
    B_PRINT_DEBUG("BThread::~BThread() - "
                  "Destruct function was called. ")
#endif
    if (m_private_ptr != nullptr) {
        delete m_private_ptr;
        m_private_ptr = nullptr;
    }
    if (m_thread_info_ptr != nullptr) {
        delete m_thread_info_ptr;
        m_thread_info_ptr = nullptr;
    }
    if (m_thread_handle != nullptr) {
        delete m_thread_handle;
        m_thread_handle = nullptr;
    }
}

BThread& BThread::operator=(BThread&& _bthread) noexcept {
#ifdef _B_DEBUG_
    B_PRINT_DEBUG("BThread::operator=(BThread&& _bthread) - "
                  "Copy function was called. ")
#endif
    // TODO(Ball Chang): BThreadPrivate should implement
    // a copy construct function.
    m_private_ptr = _bthread.m_private_ptr;
    m_thread_info_ptr = _bthread.m_thread_info_ptr;
    if (m_thread_handle != nullptr) {
        delete m_thread_handle;
        m_thread_handle = nullptr;
    }

    if (_bthread.m_thread_handle == nullptr) {
    } else {
        m_thread_handle = new thread(std::move(*(_bthread.m_thread_handle)));
    }
    _bthread.m_thread_info_ptr = nullptr;
    _bthread.m_thread_handle = nullptr;
    _bthread.m_private_ptr = nullptr;
}

bool BThread::joinable() {
    if (m_thread_handle != nullptr)
        return (*m_thread_handle).joinable();
    else
        return false;
}

bool BThread::isExit() {
#ifdef _B_DEBUG_
    B_PRINT_DEBUG("BThread::isExit - is_exited is "
    << m_thread_info_ptr->isExit()
    << " - BThreadInfo address is :" << m_thread_info_ptr)
#endif
    return m_thread_info_ptr->isExit();
}

bool BThread::isRunning() {
#ifdef _B_DEBUG_
    B_PRINT_DEBUG("BThread::isExit - is_exited is "
    << m_thread_info_ptr->isRunning()
    << " - BThreadInfo address is :" << m_thread_info_ptr)
#endif
    return m_thread_info_ptr->isRunning();
}

int32 BThread::join() {
    if (joinable()) {
        (*m_thread_handle).join();
        return BSuccess;
    } else {
        return BError;
    }
}

int32 BThread::detach() {
    if (joinable()) {
        (*m_thread_handle).detach();
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
    cpu_set_t _cpuset;
    CPU_ZERO(&_cpuset);

    int _retcode = 0;
    unsigned int _cpus = thread::hardware_concurrency();
    unsigned int _cpu_num = cpu_num % _cpus;

    CPU_SET(_cpu_num, &_cpuset);

    _retcode = pthread_setaffinity_np(nativeHandle(),
                                    sizeof(cpu_set_t), &_cpuset);
    if (_retcode != 0)
        return BCore::ReturnCode::BError;
    else
        return BCore::ReturnCode::BSuccess;
#elif defined(WIN32)
#endif
}

std::thread::id BThread::id() {
    if (m_thread_handle != nullptr) {
        if (joinable())
            return (*m_thread_handle).get_id();
        else
            return m_thread_info_ptr->id();
    } else {
        return std::thread::id();
    }
}

std::thread::native_handle_type BThread::nativeHandle() {
    if (m_thread_handle != nullptr)
        return m_thread_handle->native_handle();
    else
        return std::thread::native_handle_type();
}

#ifdef LINUX
pid_t BThread::tid() {
    pid_t null_obj;
    return null_obj;
}
#endif

// End of implementation

}  // namespace BThreadPack
