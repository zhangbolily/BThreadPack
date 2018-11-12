/*      
 * @Author      : Ball Chang
 * @File        : BThread.cpp
 * @Date        : 2018-11-11
*/


#include "BThread.h"

namespace BThreadPack{

BThread::BThread(int _detach = 0)
	:_pid_(0), _detach_(_detach)
{
}

BThread::~BThread()
{
	//Do nothing.
}

void BThread::kill()
{
	//Do nothing.
}

};
