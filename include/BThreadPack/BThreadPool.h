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
 * @File        : BThreadPool.h
 * @Date        : 2018-12-3
*/

#ifndef INCLUDE_BTHREADPACK_BTHREADPOOL_H_
#define INCLUDE_BTHREADPACK_BTHREADPOOL_H_

#ifdef WIN32
#include <windows.h>
#else
#include <sys/prctl.h>
#endif

#include <string>
#include <vector>

#include "BUtils/BTimer.h"
#include "BThreadPack/BThreadPack.h"
#include "BThreadPack/BAbstractThreadPool.h"
#include "BThreadPack/BGeneralTask.h"

namespace BThreadPack {

using std::vector;

class BThreadPoolPrivate;

class BThreadPool: public BAbstractThreadPool{
 public:
    enum Optimizer {
        None = 0,
        PerformanceFirst = 1,
        ProcessTimeFirst = 2
    };

    explicit BThreadPool(uint _thread_cap);
    BThreadPool(uint _thread_cap,
                BAbstractThreadPool::BThreadControlMode _mode);
    ~BThreadPool() override;

    /* Overload pushTask */
    void pushTask(BGeneralTask* _task_ptr);
    void pushGroupTask(BGroupTask* _task_ptr);
    void setOptimizePolicy(BThreadPool::Optimizer _policy);
    int32 optimizer(std::vector<BGeneralTask *> _task_vec,
                    BThreadPool::Optimizer _op_type);
    uint resize(uint _size) override;
    const BThreadPool::Optimizer optimizePolicy() const;

 protected:
    friend class BThreadPoolPrivate;
    BThreadPoolPrivate* m_private_ptr;

 private:
    BThreadPool::Optimizer m_optimize_policy;
};

}  // namespace BThreadPack

#endif  // INCLUDE_BTHREADPACK_BTHREADPOOL_H_
