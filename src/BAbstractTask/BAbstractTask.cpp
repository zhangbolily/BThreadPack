/*
 * @Author      : Ball Chang
 * @File        : BAbstractTask.cpp
 * @Date        : 2018-11-12
*/

#include "BAbstractTask.h"

namespace BThreadPack{

BAbstractTask::BAbstractTask()
	:_taskStatus_(BTaskStatus::TaskInit)
{
}

BAbstractTask::~BAbstractTask()
{
}

};
