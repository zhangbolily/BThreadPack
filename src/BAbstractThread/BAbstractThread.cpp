/*
 * @Author      : Ball Chang
 * @File        : BAbstractThread.cpp
 * @Date        : 2018-11-12
*/


#include "BAbstractThread.h"

namespace BThreadPack{

BAbstractThread::BAbstractThread(bool _detach = false)
    :_pid_(this_thread::get_id()), _detach_(_detach)
{
}

BAbstractThread::~BAbstractThread()
{
    //Do nothing.
}

/* @kill - Kill this thread and release all resources.
 * Don't need any parameters.
*/
void BAbstractThread::kill()
{
    //Do nothing.
}

void BAbstractThread::ThreadFunction(void * _buffer)
{
    //Do nothing.
}

};
