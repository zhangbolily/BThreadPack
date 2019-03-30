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
 * @File        : BAbstractTask.cpp
 * @Date        : 2018-11-12
*/

#include "BThreadPack/BAbstractTask.h"
#include "BThreadPack/private/BAbstractTaskPrivate.h"

namespace BThreadPack {

BAbstractTask::BAbstractTask()
        :m_task_priority((PriorityNum+1)/2),
         m_task_status_(static_cast<int>(BTaskStatus::TaskInit)),
         m_task_autodestroy(true),
         m_input_buffer_(nullptr),
         m_input_buffer_size_(0),
         m_output_buffer_(nullptr),
         m_output_buffer_size_(0),
         m_private_ptr(new BAbstractTaskPrivate) {
}

BAbstractTask::BAbstractTask(bool _autodestroy)
        :m_task_priority((PriorityNum+1)/2),
         m_task_status_(static_cast<int>(BTaskStatus::TaskInit)),
         m_task_autodestroy(_autodestroy),
         m_input_buffer_(nullptr),
         m_input_buffer_size_(0),
         m_output_buffer_(nullptr),
         m_output_buffer_size_(0),
         m_private_ptr(new BAbstractTaskPrivate) {
}

BAbstractTask::BAbstractTask(const void* _buffer, size_t _size)
        :m_task_priority((PriorityNum+1)/2),
         m_task_status_(static_cast<int>(BTaskStatus::TaskInit)),
         m_task_autodestroy(true),
         m_output_buffer_(nullptr),
         m_output_buffer_size_(0),
         m_private_ptr(new BAbstractTaskPrivate) {
    if (setInputBuffer(_buffer, _size)) {
        // TODO(Ball Chang) :SetInputBuffer failed.
        // This object won't be created.
#ifdef _B_DEBUG_
        B_PRINT_DEBUG("BAbstractTask::BAbstractTask"
                      "(void* _buffer, size_t _size)"
                      " - setInputBuffer failed.")
#endif
        m_task_status_ = static_cast<int>(BTaskStatus::TaskFailed);
        return;
    }
}

BAbstractTask::BAbstractTask(
        const void* _buffer,
        size_t _size,
        bool _autodestroy)
        :m_task_priority((PriorityNum+1)/2),
         m_task_status_(static_cast<int>(BTaskStatus::TaskInit)),
         m_task_autodestroy(_autodestroy),
         m_output_buffer_(nullptr),
         m_output_buffer_size_(0),
         m_private_ptr(new BAbstractTaskPrivate) {
    if (setInputBuffer(_buffer, _size)) {
        // TODO(Ball Chang) :SetInputBuffer failed.
        // This object won't be created.
#ifdef _B_DEBUG_
        B_PRINT_DEBUG("BAbstractTask::BAbstractTask"
                      "(void* _buffer, size_t _size)"
                      " - setInputBuffer failed.")
#endif
        m_task_status_ = static_cast<int>(BTaskStatus::TaskFailed);
        return;
    }
}

BAbstractTask::~BAbstractTask() {
    if (m_input_buffer_ != nullptr) {
        delete [] m_input_buffer_;
        m_input_buffer_ = nullptr;
    }
    if (m_output_buffer_ != nullptr) {
        delete [] m_output_buffer_;
        m_output_buffer_ = nullptr;
    }
    if (m_private_ptr != nullptr) {
        delete m_private_ptr;
        m_private_ptr = nullptr;
    }
}

int32 BAbstractTask::status() {
    if (!m_task_mutex.try_lock()) {
        return BCore::ReturnCode::BOnlySingleThread;
    }
    m_task_mutex.unlock();

    return m_task_status_;
}

int32 BAbstractTask::setStatus(BAbstractTask::BTaskStatus _status) {
    if (!m_task_mutex.try_lock()) {
        return BCore::ReturnCode::BOnlySingleThread;
    }

    m_task_status_ = static_cast<int>(_status);
    m_task_mutex.unlock();

    return BCore::ReturnCode::BSuccess;
}

int32 BAbstractTask::setStatus(std::atomic_int _status) {
    if (!m_task_mutex.try_lock()) {
        return BCore::ReturnCode::BOnlySingleThread;
    }

    m_task_status_.store(_status);
    m_task_mutex.unlock();

    return BCore::ReturnCode::BSuccess;
}

int32 BAbstractTask::setInputBuffer(const void* _buffer, const size_t _size) {
    if (!m_task_mutex.try_lock()) {
        return BCore::ReturnCode::BOnlySingleThread;
    }

    if (_buffer == nullptr) {
#ifdef _B_DEBUG_
        B_PRINT_ERROR("BAbstractTask::setInputBufer"
                      " input pointer is nullptr. ")
#endif
        return BCore::ReturnCode::BError;
    }

    if (!_size) {
#ifdef _B_DEBUG_
        B_PRINT_ERROR("BAbstractTask::setInputBufer"
                      " input buffer size is 0. ")
#endif
        return BCore::ReturnCode::BError;
    }

    // Start transaction
    m_input_buffer_ = new char[_size]();
    std::memcpy(m_input_buffer_, _buffer, _size);
    m_input_buffer_size_.store(_size);
    // Commit

    m_task_mutex.unlock();

    return BCore::ReturnCode::BSuccess;
}

int32 BAbstractTask::inputBuffer(void** _buffer, size_t &_size) {
    if (!m_task_mutex.try_lock()) {
        return BCore::ReturnCode::BOnlySingleThread;
    }

    *_buffer = m_input_buffer_;
    _size = m_input_buffer_size_.load();

    m_task_mutex.unlock();

    return BCore::ReturnCode::BSuccess;
}

int32 BAbstractTask::setOutputBuffer(void* _buffer, size_t _size) {
    if (!m_task_mutex.try_lock()) {
        return BCore::ReturnCode::BOnlySingleThread;
    }

    if (_buffer == nullptr) {
#ifdef _B_DEBUG_
        B_PRINT_ERROR("BAbstractTask::setOutputBuffer"
                      " input pointer is nullptr.")
#endif
        return BCore::ReturnCode::BError;
    }

    if (!_size) {
#ifdef _B_DEBUG_
        B_PRINT_ERROR("BAbstractTask::setOutputBuffer"
                      " input buffer size is 0. ")
#endif
        return BCore::ReturnCode::BError;
    }

    // Start transaction
    m_output_buffer_ = new char[_size]();
    std::memcpy(m_output_buffer_, _buffer, _size);
    m_output_buffer_size_.store(_size);
    // Commit

    m_task_mutex.unlock();

    return BCore::ReturnCode::BSuccess;
}

int32 BAbstractTask::outputBuffer(const void** _buffer, size_t &_size) {
    if (!m_task_mutex.try_lock()) {
        return BCore::ReturnCode::BOnlySingleThread;
    }

    *_buffer = m_output_buffer_;
    _size = m_output_buffer_size_.load();

    m_task_mutex.unlock();

    return BCore::ReturnCode::BSuccess;
}

void BAbstractTask::setPriority(int32 _priority) {
    m_task_priority = _priority;
}

int32 BAbstractTask::priority() const {
    return m_task_priority.load();
}

void BAbstractTask::startExecutionTiming() {
    m_execute_timer.start();
}

void BAbstractTask::stopExecutionTiming() {
    m_execute_timer.stop();
}

int64 BAbstractTask::executionTime() {
    return m_execute_timer.time();
}

void BAbstractTask::startRealTiming() {
    m_real_timer.start();
}

void BAbstractTask::stopRealTiming() {
    m_real_timer.stop();
}

int64 BAbstractTask::realTime() {
    return m_real_timer.time();
}

void BAbstractTask::setUUID() {
    if (!BUtils::isUUID4(m_uuid))
        m_uuid = BUtils::generateUUID4();
#ifdef _B_DEBUG_
    B_PRINT_DEBUG("BAbstractTask::setUUID - UUID is " << m_uuid << " .")
#endif
}

void BAbstractTask::setUUID(const std::string &_uuid) {
    m_uuid = _uuid;
}

const std::string BAbstractTask::UUID() {
    return m_uuid;
}

void BAbstractTask::setName(const char* _name) {
    m_name = _name;
}

void BAbstractTask::setName(const std::string _name) {
    m_name = _name;
}

void BAbstractTask::wait() {
    if (status() == BAbstractTask::BTaskStatus::TaskInit) {
        return;     // This task is not running, won't be blocked.
    } else {
        // Task is running, block until all tasks are finished.
        std::unique_lock<std::mutex> lock(m_private_ptr->m_task_mutex);
        m_private_ptr->m_task_cond.wait(lock);
    }
}

const std::string& BAbstractTask::name() const {
    return m_name;
}

bool BAbstractTask::destroyable() const {
    return m_task_autodestroy.load();
}

}   // namespace BThreadPack
