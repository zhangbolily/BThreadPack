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
 * @File        : BWorkerTask.h
 * @Date        : 2018-12-2
*/

#ifndef INCLUDE_BTHREADPACK_BWORKERTASK_H_
#define INCLUDE_BTHREADPACK_BWORKERTASK_H_

#include "BThreadPack/BThreadPack.h"
#include "BThreadPack/BAbstractTask.h"

namespace BThreadPack {

class BWorkerTask:public BAbstractTask {
 public:
    /* @BWorkerTask() - Constructor
    */
    BWorkerTask();

    /* @BWorkerTask() - Constructor
     * @_buffer - the task data buffer
     * @_size - the size of buffer
    */
    BWorkerTask(void* _buffer, size_t _size);

    /* @~BWorkerTask() - Destructor */
    ~BWorkerTask();
};

}  // namespace BThreadPack
#endif  // INCLUDE_BTHREADPACK_BWORKERTASK_H_
