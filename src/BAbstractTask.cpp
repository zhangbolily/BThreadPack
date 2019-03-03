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
    BAbstractTask(true);
}

BAbstractTask::BAbstractTask(bool _autodestroy)
{
    m_input_buffer_ = nullptr;
    m_input_buffer_size_ = 0;
    m_output_buffer_ = nullptr;
    m_output_buffer_size_ = 0;
    m_task_autodestroy = _autodestroy;
    m_task_status_ = static_cast<int>(BTaskStatus::TaskInit);
}

BAbstractTask::BAbstractTask(void* _buffer, size_t _size)
{
    BAbstractTask(true, _buffer, _size);
}

BAbstractTask::BAbstractTask(bool _autodestroy, void* _buffer, size_t _size)
{
    BAbstractTask(std::forward<bool>(_autodestroy));
    
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
    if (m_input_buffer_ != nullptr)
    {
        delete m_input_buffer_;
    }
    if (m_output_buffer_ != nullptr)
    {
        delete m_output_buffer_;
    }
}

int BAbstractTask::status()
{
    if(!m_task_mutex.try_lock())
    {
    	return ReturnCode::BOnlySingleThread;
    }
	m_task_mutex.unlock();
        
    return m_task_status_;
}

int BAbstractTask::setStatus(BAbstractTask::BTaskStatus _status)
{
	if(!m_task_mutex.try_lock())
    {
    	return ReturnCode::BOnlySingleThread;
    }

    m_task_status_ = static_cast<int>(_status);
	m_task_mutex.unlock();
    
    return ReturnCode::BSuccess;
}

int BAbstractTask::setStatus(atomic_int _status)
{
    if(!m_task_mutex.try_lock())
    {
    	return ReturnCode::BOnlySingleThread;
    }
    
    m_task_status_.store(_status);
	m_task_mutex.unlock();
    
    return ReturnCode::BSuccess;
}

int BAbstractTask::setInputBuffer(void* _buffer, size_t _size)
{
    if(!m_task_mutex.try_lock())
    {
    	return ReturnCode::BOnlySingleThread;
    }
       
    if(_buffer == nullptr)
    {
#ifdef _B_DEBUG_
        B_PRINT_ERROR("BAbstractTask::setInputBufer input pointer is nullptr. ")
#endif
        return ReturnCode::BError;
    }
    
    if(!_size)
    {
#ifdef _B_DEBUG_
        B_PRINT_ERROR("BAbstractTask::setInputBufer input buffer size is 0. ")
#endif
        return ReturnCode::BError;
    }
    
    //Start transaction
    m_input_buffer_ = static_cast<char*>(new(0) char[_size]);
    std::memcpy(m_input_buffer_, _buffer, _size);
    m_input_buffer_size_.store(_size);
    //Commit
        
	m_task_mutex.unlock();

    return ReturnCode::BSuccess;
}

int BAbstractTask::inputBuffer(void** _buffer, size_t &_size)
{
    if(!m_task_mutex.try_lock())
    {
    	return ReturnCode::BOnlySingleThread;
    }
    
    *_buffer = m_input_buffer_;
    _size = m_input_buffer_size_.load();
    
	m_task_mutex.unlock();
    
    return ReturnCode::BSuccess;
}

int BAbstractTask::setOutputBuffer(void* _buffer, size_t _size)
{
    if(!m_task_mutex.try_lock())
    {
    	return ReturnCode::BOnlySingleThread;
    }
    
    if(_buffer == nullptr)
    {
#ifdef _B_DEBUG_
        B_PRINT_ERROR("BAbstractTask::setOutputBuffer input pointer is nullptr.")
#endif
        return ReturnCode::BError;
    }
        
    if(!_size)
    {
#ifdef _B_DEBUG_
        B_PRINT_ERROR("BAbstractTask::setOutputBuffer input buffer size is 0. ")
#endif
        return ReturnCode::BError;
    }
    
    //Start transaction
    m_output_buffer_ = static_cast<char*>(new(0) char[_size]);
    std::memcpy(m_output_buffer_, _buffer, _size);
    m_output_buffer_size_.store(_size);
    //Commit
        
	m_task_mutex.unlock();
        
    return ReturnCode::BSuccess;
}

int BAbstractTask::outputBuffer(void** _buffer, size_t &_size)
{
    if(!m_task_mutex.try_lock())
    {
    	return ReturnCode::BOnlySingleThread;
    }
        
    *_buffer = m_output_buffer_;
    _size = m_output_buffer_size_.load();
    
	m_task_mutex.unlock();
    
    return ReturnCode::BSuccess;
}

bool BAbstractTask::destroyable() const
{
    return m_task_autodestroy.load();
}
};
