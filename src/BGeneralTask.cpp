/*
 * @Author      : Ball Chang
 * @File        : BGeneralTask.cpp
 * @Date        : 2018-12-3
*/

#include "BThreadPack.h"
#include "BGeneralTask.h"

namespace BThreadPack{
   
BGeneralTask::BGeneralTask()
    :BAbstractTask()
{
}

BGeneralTask::BGeneralTask(bool _autodestroy)
    :BAbstractTask(_autodestroy)
{
}
   
BGeneralTask::BGeneralTask(const void* _buffer, size_t _size)
    :BAbstractTask(_buffer, _size)
{
}

BGeneralTask::BGeneralTask(bool _autodestroy, const void* _buffer, size_t _size)
    :BAbstractTask(_autodestroy, _buffer, _size)
{
}

BGeneralTask::~BGeneralTask()
{
}

};
