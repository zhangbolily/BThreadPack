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

}

int BAbstractTask::status()
{
    lock_guard<std::mutex> guard(m_task_mutex);
        
    return m_task_status_;
}

int BAbstractTask::setStatus(BAbstractTask::BTaskStatus _status)
{
    lock_guard<std::mutex> guard(m_task_mutex);

    m_task_status_ = static_cast<int>(_status);
    
    return B_SUCCESS;
}

int BAbstractTask::setStatus(atomic_int _status)
{
    lock_guard<std::mutex> guard(m_task_mutex);
    
    m_task_status_.store(_status);
    
    return B_SUCCESS;
}

int BAbstractTask::setInputBuffer(void* _buffer, size_t _size)
{
    lock_guard<std::mutex> guard(m_task_mutex);
       
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
}

int BAbstractTask::inputBuffer(void** _buffer, size_t &_size)
{
    lock_guard<std::mutex> guard(m_task_mutex);
    
    *_buffer = m_input_buffer_;
    _size = m_input_buffer_size_.load();
    
    return B_SUCCESS;
}

int BAbstractTask::setOutputBuffer(void* _buffer, size_t _size)
{
    lock_guard<std::mutex> guard(m_task_mutex);
    
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
}

int BAbstractTask::outputBuffer(void** _buffer, size_t &_size)
{
    lock_guard<std::mutex> guard(m_task_mutex);
        
    *_buffer = m_output_buffer_;
    _size = m_output_buffer_size_.load();
    
    return B_SUCCESS;
}
};
