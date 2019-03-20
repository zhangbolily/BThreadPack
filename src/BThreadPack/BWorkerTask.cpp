/*
 * @Author      : Ball Chang
 * @File        : BWorkerTask.cpp
 * @Date        : 2018-12-2
*/

#include "BThreadPack/BWorkerTask.h"

namespace BThreadPack{

BWorkerTask::BWorkerTask()
{
}
  
BWorkerTask::BWorkerTask(void* _buffer, size_t _size)
    :BAbstractTask(_buffer, _size)
{
}

BWorkerTask::~BWorkerTask()
{
}

};
