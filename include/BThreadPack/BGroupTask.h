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

#ifndef _BGROUP_TASK_H_
#define _BGROUP_TASK_H_

#include "BThreadPack/BThreadPack.h"
#include "BThreadPack/BAbstractTask.h"

namespace BThreadPack{

class BGroupTaskPrivate;

class BGroupTask:private NoneCopy{

public:
    /* @BGroupTask() - Constructor */    
    BGroupTask();
    ~BGroupTask();
    
    int removeTask(unsigned int _task_id);
    int priority() const;
    void pushTask(BAbstractTask* _task_handle);
    void setPriority(int);
    long long executionTime();
    long long realTime();
    unsigned int size();
    BAbstractTask* getTask(unsigned int _task_id);
    const std::string UUID();
    
protected:
    BGroupTaskPrivate* m_private_ptr;
    friend class BAbstractThreadPool;
    friend class BGeneralThreadPool;
};
};

#endif
