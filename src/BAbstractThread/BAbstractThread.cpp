/*
 * @Author      : Ball Chang
 * @File        : BAbstractThread.cpp
 * @Date        : 2018-11-12
*/


#include "BAbstractThread.h"

namespace BThreadPack{

BAbstractThread::BAbstractThread(bool _detach = false)
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

void BAbstractThread::ThreadFunction()
{
    //Do nothing.
}

};
