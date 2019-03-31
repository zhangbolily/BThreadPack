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
 * @File        : BGeneralTask.h
 * @Date        : 2018-12-3
*/

#ifndef INCLUDE_BTHREADPACK_BGENERALTASK_H_
#define INCLUDE_BTHREADPACK_BGENERALTASK_H_

#include <string>

#include "BThreadPack/BThreadPack.h"
#include "BThreadPack/BAbstractTask.h"

namespace BThreadPack {

class BGeneralTask: public BAbstractTask {
 public:
    /* @BGeneralTask() - Constructor */
    BGeneralTask();
    explicit BGeneralTask(bool _autodestroy);

    /* @~BGeneralTask() - Destructor */
    virtual ~BGeneralTask();

    /* @BGeneralTask() - Constructor
     * @_buffer - the task data buffer
     * @_size - the size of buffer
    */    
    BGeneralTask(const void* _buffer, size_t _size);
    BGeneralTask(const void* _buffer, size_t _size, bool _auto_delete);

    /* @execute() - This function will call
     * _taskFunction to execute this task itself.
     */
    virtual int execute() = 0;
};

}  // namespace BThreadPack

#endif  // INCLUDE_BTHREADPACK_BGENERALTASK_H_
