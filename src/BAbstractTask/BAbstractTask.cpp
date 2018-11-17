/*
 * @Author      : Ball Chang
 * @File        : BAbstractTask.cpp
 * @Date        : 2018-11-12
*/

#include "BThreadPack.h"

namespace BThreadPack{

/* @BAbstractTask() - Constructor
 * Don't need any parameter
*/
BAbstractTask::BAbstractTask()
{
    this->_inputBuffer_ = nullptr;
    this->_outputBuffer_ = nullptr;
    this->_taskStatus_ = (int)BTaskStatus::TaskInit;
    this->_threadID_ = this_thread::get_id();
    this->_threadRefCount_ = 1;
}

/* @BAbstractTask() - Constructor
 * @_buffer - the task data buffer
*/    
BAbstractTask::BAbstractTask(void* _buffer, size_t _size)
{
    if(this->setInputBuffer(_buffer, _size))
    {
        //SetInputBuffer failed. This object won't be created.
        this->~BAbstractTask();
    }
    
    this->_outputBuffer_ = nullptr;
    this->_taskStatus_ = (int)BTaskStatus::TaskInit;
}

/* @~BAbstractTask() - Destructor
 * Don't need any parameter
*/
BAbstractTask::~BAbstractTask()
{
    this->_threadSafe();
}

/* @getTaskStatus() - Return the _taskStatus_ value
 * Don't need any parameter
*/
int BAbstractTask::getTaskStatus()
{
    /* If _threadSafe() returned none-zero, this means this object is not running in single-thread
     * mode and return an error code.
    */
    if(this->_threadSafe())
        return this->_threadSafe();
        
    return this->_taskStatus_;
}

/* @setTaskStatus() - set the _taskStatus_ value
 * @parameter status : the new value of _taskStatus_
*/
int BAbstractTask::setTaskStatus(BAbstractTask::BTaskStatus _status)
{
    /* If _threadSafe() returned none-zero, this means this object is not running in single-thread
     * mode and return an error code.
    */
    if(this->_threadSafe())
        return this->_threadSafe();
        
    this->_taskStatus_ = (int)_status;
    
    return B_SUCCESS;
}

/* @setTaskStatus() - set the _taskStatus_ value
 * @parameter status - the new value of _taskStatus_ with type atomic_int
*/
int BAbstractTask::setTaskStatus(atomic_int _status)
{
    /* If _threadSafe() returned none-zero, this means this object is not running in single-thread
     * mode and return an error code.
    */
    if(this->_threadSafe())
        return this->_threadSafe();
        
    this->_taskStatus_.store(_status, std::memory_order_relaxed);
    
    return B_SUCCESS;
}

/* @setInputBufer() - set the _inputBuffer_ value
 * @_buffer - the task input buffer
 * @return - return 0 if success
*/
int BAbstractTask::setInputBuffer(void* _buffer, size_t _size)
{
    /* If _threadSafe() returned none-zero, this means this object is not running in single-thread
     * mode and return an error code.
    */
    if(this->_threadSafe())
        return this->_threadSafe();
        
    if(_buffer == nullptr)
    {
        cerr<<"[Error] BAbstractTask::setInputBufer input pointer is nullptr. "<<endl;
        return B_ERROR;
    }else
        this->_inputBuffer_ = _buffer;
        
    if(!_size)
    {
        cerr<<"[Error] BAbstractTask::setInputBufer input buffer size is 0. "<<endl;
        this->_inputBuffer_ = nullptr;
        return B_ERROR;
    }else
        this->_inputBufferSize_.store(_size, std::memory_order_relaxed);
        
    return B_SUCCESS;
}

/* @getOutputBufer() - get the _outputBuffer_ value
 * @return - the task output buffer
*/
int BAbstractTask::getOutputBuffer(void** _buffer, size_t &_size)
{
    /* If _threadSafe() returned none-zero, this means this object is not running in single-thread
     * mode and return an error code.
    */
    if(this->_threadSafe())
        return this->_threadSafe();
        
    *_buffer = this->_outputBuffer_;
    
    return B_SUCCESS;
}

/* @_threadChanged() - check whether this object has beed moved to another thread
 * Don't need any parameter
*/
bool BAbstractTask::_threadChanged()
{
    if(this_thread::get_id() != this->_threadID_)
    {
        this->_threadRefCount_++;
        return true;
    }else
        return false;
}

/* @_threadSafe() - check the thread status of this object and make sure this object is thread-safe
 * Don't need any parameterr
 * Returned B_ONLY_SINGLE_THREAD error code if this object is running in multi-thread mode.
*/
int BAbstractTask::_threadSafe()
{
    if(this->_threadChanged())
    {
        if(_threadRefCount_ > 1)
        {
            return B_ONLY_SINGLE_THREAD;
        }
    }else{
        return B_SUCCESS;
    }
    
    return B_SUCCESS;
}

};
