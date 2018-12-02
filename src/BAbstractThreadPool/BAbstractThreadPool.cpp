/*
 * @Author      : Ball Chang
 * @File        : BAbstractThreadPool.cpp
 * @Date        : 2018-12-2
*/

#include "BAbstractThreadPool.h"

namespace BThreadPack{

BAbstractThreadPool::BAbstractThreadPool(int _thread_num)
{
}

BAbstractThreadPool::~BAbstractThreadPool()
{
    //Do nothing.
}

/* @kill - Kill all threads and release all resources.
 * Don't need any parameters.
*/
void BAbstractThreadPool::kill()
{
    //Do nothing.
}

void BAbstractThreadPool::ThreadFunction(void * _buffer)
{
    //Do nothing.
}

};
