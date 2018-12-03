/*
 * @Author      : Ball Chang
 * @File        : BAbstractThreadPool.cpp
 * @Date        : 2018-12-2
*/

#include "BAbstractThreadPool.h"

namespace BThreadPack{

/* @BAbstractThreadPool() - Constructor
 * @_thread_num - How many thread you want to create.
*/
BAbstractThreadPool::BAbstractThreadPool(int _threadNum)
    :_threadNum_(_threadNum)
{
    this->_initThreads_();
}

/* @~BAbstractThreadPool() - Destructor
 * Don't need any parameter
*/
BAbstractThreadPool::~BAbstractThreadPool()
{
    //Do nothing.
}

/* @startOneTask - Start one thread not creat it.
 * This function will notify at least one thread to pop task queue.
 * If the task queue only has one task, this function behaves like start one task.
 * However, if the task queue has more than one task, the behaviour is undefined.
 * Don't need any parameters.
 * @return - return 0 if success
*/
int BAbstractThreadPool::startOneTask()
{
    this->_startCond_.notify_one();
    
    return 0;
}

/* @startOneTask - This function will notify all threads to pop task queue.
 * @return - return 0 if success
*/
int BAbstractThreadPool::startAllTask()
{
    this->_startCond_.notify_all();
    
    return 0;
}

/* @kill - Kill all threads and release all resources.
 * Actually this function do nothing now.
 * Don't need any parameters.
*/
int BAbstractThreadPool::kill()
{
    return 0;
}

/* @addTask - Add a task to the task queue.
 * @_taskBuffer - A memory buffer that contains the task data.
 * @return - return 0 if success
*/
int BAbstractThreadPool::addTask(void* _taskBuffer)
{
    this->_taskQueue_.push(_taskBuffer);
    
    return 0;
}

/* @_initThreads_ - This function will initialize all threads.
 * @return - return 0 if success
*/
int BAbstractThreadPool::_initThreads_()
{
    //TODO: Define a temp pointer.
    void* _tempPointer = nullptr;
    
    for (unsigned int i = 0; i < this->_threadNum_; ++i) {
        this->_threadVec_.push_back(thread(BAbstractThreadPool::_threadFunction_, ref(_tempPointer)));
    }
    
    return 0;
}

void BAbstractThreadPool::_threadFunction_(void* _buffer)
{
    //Do nothing.
}

};