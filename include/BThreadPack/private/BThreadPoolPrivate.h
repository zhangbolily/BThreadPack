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
 * @File        : BThreadPoolPrivate.h
 * @Date        : 2019-03-27
*/

#ifndef INCLUDE_BTHREADPACK_PRIVATE_BTHREADPOOLPRIVATE_H_
#define INCLUDE_BTHREADPACK_PRIVATE_BTHREADPOOLPRIVATE_H_

#include <vector>
#include <string>

#include "BThreadPack/BThreadPack.h"
#include "BThreadPack/private/BAbstractThreadPoolPrivate.h"
#include "BThreadPack/BGeneralTask.h"

namespace  BThreadPack {

using std::vector;
using BCore::ReturnCode;

class BThreadPool;

class BThreadPoolPrivate : public BAbstractThreadPoolPrivate {
 public:
    explicit BThreadPoolPrivate(BThreadPool* ptr);

    uint m_max_performance_threads_;
    uint m_min_time_threads_;

    virtual int64 initializeThreadPool();
    virtual int64 initializeThreadPool(uint _thread_num);
    int32 normalOptimizer(std::vector<BGeneralTask *> _task_vec);

    static void Run(BThreadInfo& thread_info);
    static void setThreadName(const char* _name);
    static void setThreadName(const std::string _name);

 protected:
    BThreadPool* m_public_ptr;
};
}  // namespace BThreadPack

#endif  // INCLUDE_BTHREADPACK_PRIVATE_BTHREADPOOLPRIVATE_H_
