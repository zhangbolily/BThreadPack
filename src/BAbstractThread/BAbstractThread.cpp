/*      
 * @Author      : Ball Chang
 * @File        : BAbstractThread.cpp
 * @Date        : 2018-11-11
*/


#include "BAbstractThread.h"

namespace BThreadPack{

BAbstractThread::BAbstractThread(int _detach = 0)
	:_pid_(0), _detach_(_detach)
{
}

BAbstractThread::~BAbstractThread()
{
	//Do nothing.
}

void BAbstractThread::kill()
{
	//Do nothing.
}

};
