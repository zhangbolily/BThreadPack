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
 * @File        : BGeneralTask.cpp
 * @Date        : 2018-12-3
*/

#include "BThreadPack/BGeneralTask.h"
#include "BThreadPack/private/BAbstractTaskPrivate.h"

namespace BThreadPack {

BGeneralTask::BGeneralTask()
        :BAbstractTask() {
    m_private_ptr->setUUID();
}

BGeneralTask::BGeneralTask(bool _auto_delete)
        :BAbstractTask(_auto_delete) {
    m_private_ptr->setUUID();
}

BGeneralTask::BGeneralTask(const void* _buffer, size_t _size)
        :BAbstractTask(_buffer, _size) {
    m_private_ptr->setUUID();
}

BGeneralTask::BGeneralTask(const void* _buffer, size_t _size, bool _auto_delete)
        :BAbstractTask(_buffer, _size, _auto_delete) {
    m_private_ptr->setUUID();
}

BGeneralTask::~BGeneralTask() {
}

}  // namespace BThreadPack
