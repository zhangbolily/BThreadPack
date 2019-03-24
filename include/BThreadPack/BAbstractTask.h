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
 * @File        : BAbstractTask.h
 * @Date        : 2018-11-12
*/

#ifndef _BABSTRACT_TASK_H_
#define _BABSTRACT_TASK_H_

#include <atomic>
#include <condition_variable>
#include <cstring>
#include <iostream>
#include <map>
#include <mutex>
#include <thread>
#include <string>

#include "BUtils/BTimer.h"
#include "BUtils/BUtils.h"
#include "BThreadPack/BThreadPack.h"
#include "BThreadPack/BAbstractClass.h"

using namespace std;

extern const int PriorityNum;

namespace BThreadPack {

class BAbstractTaskPrivate;

class BAbstractTask: private NoneCopy{

public:
    enum BTaskStatus{
        TaskFailed = -1,
        TaskInit = 0,
        TaskExecuting = 1,
        TaskFinished = 2
    };

    BAbstractTask();
    BAbstractTask(bool _autodestroy);
    BAbstractTask(const void* _buffer, size_t _size);
    BAbstractTask(const void* _buffer, size_t _size, bool _autodestroy);
    virtual ~BAbstractTask();
    int status();
    int setStatus(BTaskStatus _status);
    int setStatus(std::atomic_int _status);
    int setInputBuffer(const void* _buffer, size_t _size);
    int inputBuffer(void** _buffer, size_t &_size);
    int setOutputBuffer(void* _buffer, size_t _size);
    int outputBuffer(const void** _buffer, size_t &_size);
    void setPriority(int);
    int priority() const;
    void startExecutionTiming();
    void stopExecutionTiming();
    long long executionTime();
    void startRealTiming();
    void stopRealTiming();
    long long realTime();
    void setName(const char* _name);
    void setName(std::string _name);
    const std::string& name() const;
    void wait();
    void setUUID();
    void setUUID(std::string &_uuid);
    const std::string UUID();
    bool destroyable() const;
    std::mutex m_task_mutex;

protected:
    BAbstractTaskPrivate* m_private_ptr;
  
private:

    /* @m_task_status_ - store the status of task */
    std::atomic_int m_task_priority;
    std::atomic_int m_task_status_;
    std::atomic_bool m_task_autodestroy;
    BTimer m_real_timer;
    BTimer m_execute_timer;
    std::string m_name;
    std::string m_uuid;
    
    /* @m_input_buffer_ - store the buffer of input data */
    char* m_input_buffer_;
    
    /* @m_input_buffer_size_ - store the size of input data buffer */
    std::atomic_size_t m_input_buffer_size_;
    
    /* @m_output_buffer_ - store the buffer of output data */
    char* m_output_buffer_;
    
    /* @m_output_buffer_size_ - store the size of output data buffer */
    std::atomic_size_t m_output_buffer_size_;
};

};

#endif
