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
 * @File        : BGroupTaskPrivate.h
 * @Date        : 2019-3-21
*/

#ifndef _BGROUP_TASK_PRIVATE_H_
#define _BGROUP_TASK_PRIVATE_H_

#include <vector>
#include <queue>

#include "BUtils/BTimer.h"
#include "BUtils/BUtils.h"
#include "BThreadPack/BThreadPack.h"
#include "BThreadPack/BAbstractTask.h"

namespace BThreadPack{

class BGroupTaskPrivate{
public:
    BGroupTaskPrivate();
    ~BGroupTaskPrivate();

    bool queueEmpty();
    void startExecutionTiming();
    void stopExecutionTiming();
    void startRealTiming();
    void stopRealTiming();
    void setUUID();
    void setUUID(std::string &_uuid);
    BAbstractTask* getTask();
    
    BTimer m_real_timer;
    BTimer m_execute_timer;
    atomic_int m_task_priority;
    std::string m_uuid;
    std::vector<BAbstractTask*> m_task_vec;
    std::queue<BAbstractTask*> m_task_queue;
};

};

#endif
