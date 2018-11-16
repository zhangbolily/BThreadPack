/*
 * @Author      : Ball Chang
 * @File        : BAbstractTask.cpp
 * @Date        : 2018-11-12
*/

#include "BAbstractTask.h"

namespace BThreadPack{

/* @BAbstractTask() - Constructor
 * Don't need any parameters
*/
BAbstractTask::BAbstractTask()
{
    this->_taskStatus_ = (int)BTaskStatus::TaskInit;
}

/* @~BAbstractTask() - Destructor
 * Don't need any parameters
*/
BAbstractTask::~BAbstractTask()
{
}

/* @getTaskStatus() - Return the _taskStatus_ value
 * Don't need any parameters
*/
int BAbstractTask::getTaskStatus() const
{
    return this->_taskStatus_;
}

/* @setTaskStatus() - set the _taskStatus_ value
 * @parameter status : the new value of _taskStatus_
*/
void BAbstractTask::setTaskStatus(BAbstractTask::BTaskStatus _status)
{
    this->_taskStatus_ = (int)_status;
}

/* @getTaskData() - Return the _taskData_ value
 * Don't need any parameters
*/
int BAbstractTask::getTaskData() const
{
    return this->_taskData_;
}

/* @setTaskData() - set the _taskData_ value
 * @_data - the task data
*/
void BAbstractTask::setTaskData(int _data)
{
    this->_taskData_ = _data;
}

};
