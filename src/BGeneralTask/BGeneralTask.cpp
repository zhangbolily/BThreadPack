/*
 * @Author      : Ball Chang
 * @File        : BGeneralTask.cpp
 * @Date        : 2018-12-3
*/

#include "BThreadPack.h"
#include "BGeneralTask.h"

namespace BThreadPack{
   
BGeneralTask::BGeneralTask()
{
}
   
BGeneralTask::BGeneralTask(void* _buffer, size_t _size)
    :BAbstractTask(_buffer, _size)
{
}

BGeneralTask::~BGeneralTask()
{
}

int BGeneralTask::processTask()
{
    //Define how to process this task
    return B_SUCCESS;
}

};