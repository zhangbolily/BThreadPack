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
 * @File        : BGroupTask.h
 * @Date        : 2019-3-17
*/

#ifndef INCLUDE_BTHREADPACK_BGROUPTASK_H_
#define INCLUDE_BTHREADPACK_BGROUPTASK_H_

#include <chrono>
#include <string>

#include "BThreadPack/BThreadPack.h"
#include "BThreadPack/BAbstractTask.h"

namespace BThreadPack{

using std::mutex;
using std::lock_guard;

class BGroupTaskPrivate;

class BGroupTask:private NoneCopy{
 public:
    enum BGroupTaskStatus {
        Failed = -1,
        Init = 0,
        Pending = 1,
        Executing = 2,
        Finished = 3
    };

    /* @BGroupTask() - Constructor */
    BGroupTask();
    virtual ~BGroupTask();

    int32 priority() const;
    int32 wait();
    void pushTask(BAbstractTask* _task_handle);
    void setPriority(int32);
    int64 executionTime();
    int64 realTime();
    uint size();
    BAbstractTask* getFinishedTask();
    const std::string UUID();

 protected:
    BGroupTaskPrivate* m_private_ptr;
    friend class BAbstractThreadPool;
    friend class BAbstractThreadPoolPrivate;
    friend class BThreadPool;
    friend class BThreadPoolPrivate;
};
}  // namespace BThreadPack

#endif  // INCLUDE_BTHREADPACK_BGROUPTASK_H_
