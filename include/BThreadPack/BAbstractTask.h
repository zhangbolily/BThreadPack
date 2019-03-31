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

#ifndef INCLUDE_BTHREADPACK_BABSTRACTTASK_H_
#define INCLUDE_BTHREADPACK_BABSTRACTTASK_H_

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

extern const int32 PriorityNum;

namespace BThreadPack {

using std::atomic;
using std::string;

using BUtils::BTimer;

class BAbstractTaskPrivate;

class BAbstractTask: private NoneCopy {
 public:
        enum BTaskStatus{
            TaskFailed = -1,
            TaskInit = 0,
            TaskExecuting = 1,
            TaskFinished = 2
        };

        BAbstractTask();
        explicit BAbstractTask(bool _auto_delete);
        BAbstractTask(const void* _buffer, size_t _size);
        BAbstractTask(const void* _buffer, size_t _size, bool _auto_delete);
        virtual ~BAbstractTask();
        int32 status();
        int32 setInputBuffer(const void* _buffer, size_t _size);
        int32 inputBuffer(void** _buffer, size_t &_size);
        int32 setOutputBuffer(void* _buffer, const size_t _size);
        int32 outputBuffer(const void** _buffer, size_t &_size);
        int32 priority() const;
        int64 executionTime();
        int64 realTime();
        void setPriority(int32);
        void setAutoDelete(bool);
        void setName(const char* _name);
        void setName(const std::string _name);
        void wait();
        const std::string UUID();
        const std::string& name() const;
        bool autoDelete() const;

 protected:
        BAbstractTaskPrivate* m_private_ptr;
        friend class BThreadPool;
        friend class BThreadPoolPrivate;
        friend class BGroupTask;
};
}   // namespace BThreadPack

#endif  // INCLUDE_BTHREADPACK_BABSTRACTTASK_H_
