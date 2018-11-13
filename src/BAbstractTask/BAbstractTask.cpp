/*
 * @Author      : Ball Chang
 * @File        : BAbstractTask.cpp
 * @Date        : 2018-11-12
*/

#include "BAbstractTask.h"

namespace BThreadPack{

BAbstractTask::BAbstractTask()
{
		this->_taskStatus_ = (int)BTaskStatus::TaskInit;
}

BAbstractTask::~BAbstractTask()
{
}

};
