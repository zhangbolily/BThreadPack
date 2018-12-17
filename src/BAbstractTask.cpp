/*
 * @Author      : Ball Chang
 * @File        : BAbstractTask.cpp
 * @Date        : 2018-11-12
*/

#include "BThreadPack.h"
#include "BAbstractTask.h"

namespace BThreadPack{

BAbstractTask::BAbstractTask()
{
    m_input_buffer_ = nullptr;
    m_input_buffer_size_ = 0;
    m_output_buffer_ = nullptr;
    m_output_buffer_size_ = 0;
    m_task_status_ = static_cast<int>(BTaskStatus::TaskInit);
    m_creater_thread_id_ = this_thread::get_id();
    m_thread_ref_count_ = 1;
}
   
BAbstractTask::BAbstractTask(void* _buffer, size_t _size)
{
    BAbstractTask();
    
    if(setInputBuffer(_buffer, _size))
    {
        //TODO
        //SetInputBuffer failed. This object won't be created.
#ifdef _B_DEBUG_
        B_PRINT_DEBUG("BAbstractTask::BAbstractTask(void* _buffer, size_t _size) - setInputBuffer failed.")
#endif
        m_task_status_ = static_cast<int>(BTaskStatus::TaskFailed);
        return;
    }
    
    m_output_buffer_ = nullptr;
    m_task_status_ = static_cast<int>(BTaskStatus::TaskInit);
}

BAbstractTask::~BAbstractTask()
{
    m_threadSafe_();
}

int BAbstractTask::status()
{
    /* If _threadSafe_() returned none-zero, this means this object is not running in single-thread
     * mode and return an error code.
    */
    if(m_threadSafe_())
        return m_threadSafe_();
        
    return m_task_status_;
}

int BAbstractTask::setStatus(BAbstractTask::BTaskStatus _status)
{
    /* If _threadSafe_() returned none-zero, this means this object is not running in single-thread
     * mode and return an error code.
    */
    if(m_threadSafe_())
        return m_threadSafe_();
    
    if(m_isWorkerThread_(this_thread::get_id()) || (m_thread_ref_count_ == 1))
    {    
        m_task_status_ = static_cast<int>(_status);
        
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
    if(m_threadSafe_())
        return m_threadSafe_();
        
    if(m_isWorkerThread_(this_thread::get_id()) || (m_thread_ref_count_ == 1))
    {    
        m_task_status_.store(_status);
        
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
    if(m_threadSafe_())
        return m_threadSafe_();
    
    if(m_isCreaterThread_(this_thread::get_id()))
    {    
        if(_buffer == nullptr)
        {
#ifdef _B_DEBUG_
            B_PRINT_ERROR("BAbstractTask::setInputBufer input pointer is nullptr. ")
#endif
            return B_ERROR;
        }else
            m_input_buffer_ = _buffer;
            
        if(!_size)
        {
#ifdef _B_DEBUG_
            B_PRINT_ERROR("BAbstractTask::setInputBufer input buffer size is 0. ")
#endif
            m_input_buffer_ = nullptr;
            return B_ERROR;
        }else
            m_input_buffer_size_.store(_size);
            
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
    if(m_threadSafe_())
        return m_threadSafe_();
    
    if(m_isWorkerThread_(this_thread::get_id()) || (m_thread_ref_count_ == 1))
    {    
        *_buffer = m_input_buffer_;
        _size = m_input_buffer_size_.load();
        
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
    if(m_threadSafe_())
        return m_threadSafe_();
    
    if(m_isWorkerThread_(this_thread::get_id()) || (m_thread_ref_count_ == 1))
    {
        if(_buffer == nullptr)
        {
#ifdef _B_DEBUG_
            B_PRINT_ERROR("BAbstractTask::setOutputBuffer input pointer is nullptr.")
#endif
            cerr<<"[Error]  "<<endl;
            return B_ERROR;
        }else
            m_output_buffer_ = _buffer;
            
        if(!_size)
        {
#ifdef _B_DEBUG_
            B_PRINT_ERROR("BAbstractTask::setOutputBuffer input buffer size is 0. ")
#endif
            m_output_buffer_ = nullptr;
            return B_ERROR;
        }else
            m_output_buffer_size_.store(_size);
            
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
    if(m_threadSafe_())
        return m_threadSafe_();
        
    if(m_isCreaterThread_(this_thread::get_id()))
    {
        *_buffer = m_output_buffer_;
        _size = m_output_buffer_size_.load();
        
        return B_SUCCESS;
    }else{
        return B_NOT_IN_CREATE_THREAD;
    }
}

bool BAbstractTask::m_isCreaterThread_(thread::id _thread_id)
{
    if(_thread_id != m_creater_thread_id_)
    {
        return false;
    }else{
        return true;
    }
}

bool BAbstractTask::m_isWorkerThread_(thread::id _thread_id)
{
    if(_thread_id != m_worker_thread_id_)
    {
        return false;
    }else{
        return true;
    }
}

bool BAbstractTask::m_threadChanged_()
{   
    bool _createThreadRet = false, _workThreadRet = false;
 
    if(!m_isCreaterThread_(this_thread::get_id()))
    {
#ifdef _B_DEBUG_
        B_PRINT_DEBUG("BAbstractTask::_threadChanged_() - Thread change detected. But the new thread is not create thread.")
        B_PRINT_DEBUG("BAbstractTask::_threadChanged_() - Create thread id:"<<m_creater_thread_id_)
        B_PRINT_DEBUG("BAbstractTask::_threadChanged_() - Current thread counter :"<<m_thread_ref_count_)
#endif
         /* The second thread obtained this object will be regard as work thread.
        */
        if(m_thread_ref_count_ == 1)
        {
            m_worker_thread_id_ = this_thread::get_id();
            m_thread_ref_count_++;
            _createThreadRet = false;
        }else
            _createThreadRet = true;
            
    }else
        _createThreadRet = false;
        
    if(!m_isWorkerThread_(this_thread::get_id()))
    {
#ifdef _B_DEBUG_
        B_PRINT_DEBUG("BAbstractTask::_threadChanged_() - Thread change detected. But the new thread is not work thread.")
        B_PRINT_DEBUG("BAbstractTask::_threadChanged_() - Work thread id:"<<m_worker_thread_id_)
        B_PRINT_DEBUG("BAbstractTask::_threadChanged_() - Current thread counter :"<<m_thread_ref_count_)
#endif
        _workThreadRet = true;
    }else
        _workThreadRet = false;
        
    return (_createThreadRet && _workThreadRet);
}

int BAbstractTask::m_threadSafe_()
{
    if(m_threadChanged_())
    {
        return B_ONLY_SINGLE_THREAD;
    }else{
        return B_SUCCESS;
    }
}

};
