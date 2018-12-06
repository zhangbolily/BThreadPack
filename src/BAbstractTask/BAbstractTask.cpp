/*
 * @Author      : Ball Chang
 * @File        : BAbstractTask.cpp
 * @Date        : 2018-11-12
*/

#include "BThreadPack.h"
#include "BAbstractTask.h"

namespace BThreadPack{

/* @BAbstractTask() - Constructor
 * Don't need any parameter
*/
BAbstractTask::BAbstractTask()
{
    this->_inputBuffer_ = nullptr;
    this->_inputBufferSize_ = 0;
    this->_outputBuffer_ = nullptr;
    this->_outputBufferSize_ = 0;
    this->_taskStatus_ = (int)BTaskStatus::TaskInit;
    this->_createThreadID_ = this_thread::get_id();
    this->_threadRefCount_ = 1;
    
#ifdef _B_DEBUG_
    B_PRINT_DEBUG("BAbstractTask::BAbstractTask() - Constructing object.")
    B_PRINT_DEBUG("BAbstractTask::BAbstractTask() - Current thread counter :"<<this->_threadRefCount_)
#endif
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

BAbstractTask::~BAbstractTask()
{
    this->_threadSafe_();
}

int BAbstractTask::status()
{
    /* If _threadSafe_() returned none-zero, this means this object is not running in single-thread
     * mode and return an error code.
    */
    if(this->_threadSafe_())
        return this->_threadSafe_();
        
    return this->_taskStatus_;
}

/* @setTaskStatus() - set the _taskStatus_ value
 * @parameter status : the new value of _taskStatus_
*/
int BAbstractTask::setStatus(BAbstractTask::BTaskStatus _status)
{
    /* If _threadSafe_() returned none-zero, this means this object is not running in single-thread
     * mode and return an error code.
    */
    if(this->_threadSafe_())
        return this->_threadSafe_();
    
    if(this->_isWorkThread_(this_thread::get_id()) || (this->_threadRefCount_ == 1))
    {    
        this->_taskStatus_ = (int)_status;
        
        return B_SUCCESS;
    }else{
        return B_NOT_IN_CREATE_THREAD;
    }
}

int BAbstractTask::setStatus(atomic_int _status)
{
    /* If _threadSafe_() returned none-zero, this means this object is not running in single-thread
     * mode and return an error code.
    */
    if(this->_threadSafe_())
        return this->_threadSafe_();
        
    if(this->_isWorkThread_(this_thread::get_id()) || (this->_threadRefCount_ == 1))
    {    
        this->_taskStatus_.store(_status);
        
        return B_SUCCESS;
    }else{
        return B_NOT_IN_CREATE_THREAD;
    }
}

int BAbstractTask::setInputBuffer(void* _buffer, size_t _size)
{
    /* If _threadSafe_() returned none-zero, this means this object is not running in single-thread
     * mode and return an error code.
    */
    if(this->_threadSafe_())
        return this->_threadSafe_();
    
    if(this->_isCreateThread_(this_thread::get_id()))
    {    
        if(_buffer == nullptr)
        {
#ifdef _B_DEBUG_
            B_PRINT_ERROR("BAbstractTask::setInputBufer input pointer is nullptr. ")
#endif
            return B_ERROR;
        }else
            this->_inputBuffer_ = _buffer;
            
        if(!_size)
        {
#ifdef _B_DEBUG_
            B_PRINT_ERROR("BAbstractTask::setInputBufer input buffer size is 0. ")
#endif
            this->_inputBuffer_ = nullptr;
            return B_ERROR;
        }else
            this->_inputBufferSize_.store(_size);
            
        return B_SUCCESS;
    }else{
        return B_NOT_IN_CREATE_THREAD;
    }
}

int BAbstractTask::inputBuffer(void** _buffer, size_t &_size)
{
    /* If _threadSafe_() returned none-zero, this means this object is not running in single-thread
     * mode and return an error code.
    */
    if(this->_threadSafe_())
        return this->_threadSafe_();
    
    if(this->_isWorkThread_(this_thread::get_id()) || (this->_threadRefCount_ == 1))
    {    
        *_buffer = this->_inputBuffer_;
        _size = this->_inputBufferSize_.load();
        
        return B_SUCCESS;
    }else{
        return B_NOT_IN_WORK_THREAD;
    }
}

int BAbstractTask::setOutputBuffer(void* _buffer, size_t _size)
{
    /* If _threadSafe_() returned none-zero, this means this object is not running in single-thread
     * mode and return an error code.
    */
    if(this->_threadSafe_())
        return this->_threadSafe_();
    
    if(this->_isWorkThread_(this_thread::get_id()) || (this->_threadRefCount_ == 1))
    {
        if(_buffer == nullptr)
        {
#ifdef _B_DEBUG_
            B_PRINT_ERROR("BAbstractTask::setOutputBuffer input pointer is nullptr.")
#endif
            cerr<<"[Error]  "<<endl;
            return B_ERROR;
        }else
            this->_outputBuffer_ = _buffer;
            
        if(!_size)
        {
#ifdef _B_DEBUG_
            B_PRINT_ERROR("BAbstractTask::setOutputBuffer input buffer size is 0. ")
#endif
            this->_outputBuffer_ = nullptr;
            return B_ERROR;
        }else
            this->_outputBufferSize_.store(_size);
            
        return B_SUCCESS;
    }else{
        return B_NOT_IN_WORK_THREAD;
    }
}

int BAbstractTask::outputBuffer(void** _buffer, size_t &_size)
{
    /* If _threadSafe_() returned none-zero, this means this object is not running in single-thread
     * mode and return an error code.
    */
    if(this->_threadSafe_())
        return this->_threadSafe_();
        
    if(this->_isCreateThread_(this_thread::get_id()))
    {
        *_buffer = this->_outputBuffer_;
        _size = this->_outputBufferSize_.load();
        
        return B_SUCCESS;
    }else{
        return B_NOT_IN_CREATE_THREAD;
    }
}

bool BAbstractTask::_isCreateThread_(thread::id _threadID)
{
    if(this_thread::get_id() != this->_createThreadID_)
    {
        return false;
    }else{
        return true;
    }
}

bool BAbstractTask::_isWorkThread_(thread::id _threadID)
{
    if(this_thread::get_id() != this->_workThreadID_)
    {
        return false;
    }else{
        return true;
    }
}

bool BAbstractTask::_threadChanged_()
{   
    bool _createThreadRet = false, _workThreadRet = false;
 
    if(!this->_isCreateThread_(this_thread::get_id()))
    {
#ifdef _B_DEBUG_
        B_PRINT_DEBUG("BAbstractTask::_threadChanged_() - Thread change detected. But the new thread is not create thread.")
        B_PRINT_DEBUG("BAbstractTask::_threadChanged_() - Create thread id:"<<this->_createThreadID_)
        B_PRINT_DEBUG("BAbstractTask::_threadChanged_() - Current thread counter :"<<this->_threadRefCount_)
#endif
         /* The second thread obtained this object will be regard as work thread.
        */
        if(this->_threadRefCount_ == 1)
        {
            this->_workThreadID_ = this_thread::get_id();
            this->_threadRefCount_++;
            _createThreadRet = false;
        }else
            _createThreadRet = true;
            
    }else
        _createThreadRet = false;
        
    if(!this->_isWorkThread_(this_thread::get_id()))
    {
#ifdef _B_DEBUG_
        B_PRINT_DEBUG("BAbstractTask::_threadChanged_() - Thread change detected. But the new thread is not work thread.")
        B_PRINT_DEBUG("BAbstractTask::_threadChanged_() - Work thread id:"<<this->_workThreadID_)
        B_PRINT_DEBUG("BAbstractTask::_threadChanged_() - Current thread counter :"<<this->_threadRefCount_)
#endif
        _workThreadRet = true;
    }else
        _workThreadRet = false;
        
    return (_createThreadRet && _workThreadRet);
}

int BAbstractTask::_threadSafe_()
{
    if(this->_threadChanged_())
    {
        return B_ONLY_SINGLE_THREAD;
    }else{
        return B_SUCCESS;
    }
}

};
