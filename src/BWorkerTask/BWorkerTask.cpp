/*
 * @Author      : Ball Chang
 * @File        : BWorkerTask.cpp
 * @Date        : 2018-12-2
*/

#include "BThreadPack.h"
#include "BWorkerTask.h"

namespace BThreadPack{

/* @BWorkerTask() - Constructor
 * Don't need any parameter
*/    
BWorkerTask::BWorkerTask()
{
}

/* @BWorkerTask() - Constructor
 * @_buffer - the task data buffer
 * @_size - the size of buffer
*/    
BWorkerTask::BWorkerTask(void* _buffer, size_t _size)
    :BAbstractTask(_buffer, _size)
{
}

/* @~BWorkerTask() - Destructor
 * Don't need any parameter
*/
BWorkerTask::~BWorkerTask()
{
}
};
