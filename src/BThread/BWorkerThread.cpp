/*
 * @Author      : Ball Chang
 * @File        : BWorkerThread.cpp
 * @Date        : 2018-11-11
*/


#include "BWorkerThread.h"

namespace BThreadPack{

BWorkerThread::BWorkerThread(bool _detach = false)
				:BAbstractThread(_detach)
{
	//Do nothing.
}

BWorkerThread::~BWorkerThread()
{
	//Do nothing.
}

void BWorkerThread::kill()
{
	//Do nothing.
}

};
