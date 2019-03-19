/*
 * @Author      : Ball Chang
 * @File        : BAbstractTask.cpp
 * @Date        : 2018-11-12
*/

#include "BThreadPack.h"
#include "BAbstractTask.h"

namespace BThreadPack{

BAbstractTask::BAbstractTask()
    :m_task_priority((PriorityNum+1)/2),
    m_task_status_(static_cast<int>(BTaskStatus::TaskInit)),
    m_task_autodestroy(true),
    m_input_buffer_(nullptr),
    m_input_buffer_size_(0),
    m_output_buffer_(nullptr),
    m_output_buffer_size_(0)
{
}

BAbstractTask::BAbstractTask(bool _autodestroy)
    :m_task_priority((PriorityNum+1)/2),
    m_task_status_(static_cast<int>(BTaskStatus::TaskInit)),
    m_task_autodestroy(_autodestroy),
    m_input_buffer_(nullptr),
    m_input_buffer_size_(0),
    m_output_buffer_(nullptr),
    m_output_buffer_size_(0)
{

}

BAbstractTask::BAbstractTask(const void* _buffer, size_t _size)
    :m_task_priority((PriorityNum+1)/2),
    m_task_status_(static_cast<int>(BTaskStatus::TaskInit)),
    m_task_autodestroy(true),
    m_output_buffer_(nullptr),
    m_output_buffer_size_(0)
{
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
}

BAbstractTask::BAbstractTask(bool _autodestroy, const void* _buffer, size_t _size)
    :m_task_priority((PriorityNum+1)/2),
    m_task_status_(static_cast<int>(BTaskStatus::TaskInit)),
    m_task_autodestroy(_autodestroy),
    m_output_buffer_(nullptr),
    m_output_buffer_size_(0)
{   
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
}

BAbstractTask::~BAbstractTask()
{
    if (m_input_buffer_ != nullptr)
    {
        delete [] m_input_buffer_;
    }
    if (m_output_buffer_ != nullptr)
    {
        delete [] m_output_buffer_;
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

int BAbstractTask::setInputBuffer(const void* _buffer, size_t _size)
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
    m_input_buffer_ = static_cast<char*>(new char[_size]);
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

int BAbstractTask::outputBuffer(const void** _buffer, size_t &_size)
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

void BAbstractTask::setPriority(int _priority)
{
    m_task_priority = _priority;
}
    
int BAbstractTask::priority() const
{
    return m_task_priority.load();
}

bool BAbstractTask::destroyable() const
{
    return m_task_autodestroy.load();
}
};
