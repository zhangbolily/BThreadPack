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
}

/* @~BAbstractThreadPool() - Destructor
 * Don't need any parameter
*/
BAbstractThreadPool::~BAbstractThreadPool()
{
    //Do nothing.
}

/* @getThreadNum - Get how many threads in this pool.
 * @return - return thread number.
*/
unsigned int BAbstractThreadPool::getThreadNum()
{
    return this->_threadNum_;
}

/* @addThread - Add a thread to the thread pool.
 * @return - return current thread number.
*/
int BAbstractThreadPool::addThread(thread _newThread)
{
    /*TODO: Some bugs here.
     *Can't control over created threads.
    */
    if(this->_threadNum_ == this->_threadVec_.size())
    {
        this->_threadVec_.push_back(std::move(_newThread));
        return this->_threadNum_;
    }
    else{
        this->_threadVec_.push_back(std::move(_newThread));
        return this->_threadNum_;
    }
    return 0;
}

/* @_initThreads_ - This function will initialize all threads.
 * @_this - Pass a fake this pointer into thread.
 * @return - return 0 if success
*/
int BAbstractThreadPool::initThreads(BAbstractThreadPool* _this)
{    
    for (unsigned int i = 0; i < this->getThreadNum(); ++i) {
        this->addThread(thread(BAbstractThreadPool::_threadFunction_, ref(_this)));
    }
    
    return 0;
}

/* @waitCond - This function wait the signal of condition_variable. 
 * No return value.
*/
void BAbstractThreadPool::waitCond()
{
    std::unique_lock<std::mutex> lock(this->_startMut_);
    this->_startCond_.wait(lock);
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
    lock_guard<std::mutex> guard(_taskMut_);
    this->_taskQueue_.push(_taskBuffer);
    
    return 0;
}

/* @getTask - Get the first task from task queue and remove it from task queue.
 * @return - return task buffer.
*/
void* BAbstractThreadPool::getTask()
{
    lock_guard<std::mutex> guard(_taskMut_);
    
    if(!this->_taskQueue_.size())
        return nullptr;
    else{
        void* _resultTask = this->_taskQueue_.front();
        this->_taskQueue_.pop();
        
        return _resultTask;
    }
}

void BAbstractThreadPool::_threadFunction_(void* _buffer)
{
    //Do nothing.
}

};
