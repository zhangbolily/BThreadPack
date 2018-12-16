/*
 * @Author      : Ball Chang
 * @File        : BAbstractTask.h
 * @Date        : 2018-11-12
*/

#ifndef _BABSTRACT_TASK_H_
#define _BABSTRACT_TASK_H_

#include <atomic>
#include <condition_variable>
#include <iostream>
#include <map>
#include <mutex>
#include <thread>

using namespace std;

namespace BThreadPack {

class BAbstractTask{

public:
    enum BTaskStatus{
        TaskFailed = -1,
        TaskInit = 0,
        TaskProcessing = 1,
        TaskFinished = 2
    };

    /* @BAbstractTask() - Constructor */    
    BAbstractTask();
    
    /* @BAbstractTask() - Constructor
     * @_buffer - the task data buffer
     * @_size - size of the buffer
    */    
    BAbstractTask(void* _buffer, size_t _size);
	
    /* @~BAbstractTask() - Destructor */
    ~BAbstractTask();

    /* @status() - Return the value of m_task_status_ */
    int status();

    /* @setStatus() - set the m_task_status_
     * @parameter _status - the new value of m_task_status_
    */
    int setStatus(BTaskStatus _status);
    
    /* @setStatus() - set the m_task_status_
     * @parameter _status - the new value of m_task_status_
    */
    int setStatus(atomic_int _status);
    
    /* @setInputBufer() - set the m_input_buffer_ value
     *  This function only works in creater thread
     * @_buffer - the task input buffer
     * @_size - size of the buffer
     * @return - returns B_SUCCESS if success
    */
    int setInputBuffer(void* _buffer, size_t _size);
    
    /* @inputBuffer() - get the m_input_buffer_ address
     *  This function only works in worker thread
     * @_buffer - the task input buffer
     * @_size - the size of buffer
     * @return - returns B_SUCCESS if success
    */
    int inputBuffer(void** _buffer, size_t &_size);
    
    /* @setOutputBufer() - set the m_output_buffer_ value
     *  This function only works in worker thread
     * @_buffer - the task output buffer
     * @_size - size of the buffer
     * @return - returns B_SUCCESS if success
    */
    int setOutputBuffer(void* _buffer, size_t _size);
    
    /* @outputBuffer() - get the m_output_buffer_ value
     *  This function only works in creater thread
     * @_buffer - the task output buffer
     * @_size - the size of buffer
     * @return - returns B_SUCCESS if success
    */
    int outputBuffer(void** _buffer, size_t &_size);
    
private:

    /* @m_task_status_ - store the status of task */
    atomic_int m_task_status_;
    
    /* @m_input_buffer_ - store the buffer of input data */
    void* m_input_buffer_;
    
    /* @m_input_buffer_size_ - store the size of input data buffer */
    atomic_size_t m_input_buffer_size_;
    
    /* @m_output_buffer_ - store the buffer of output data */
    void* m_output_buffer_;
    
    /* @m_output_buffer_size_ - store the size of output data buffer */
    atomic_size_t m_output_buffer_size_;
    
    /* @m_thread_ref_count_ - store how many threads are using this object */
    atomic_int m_thread_ref_count_;
    
    /* @m_creater_thread_id_ - store which thread create this object */
    thread::id m_creater_thread_id_;
    
    /* @m_worker_thread_id_ - store which thread use this object */
    thread::id m_worker_thread_id_;
    
    /* @m_isCreaterThread_ - check whether this thread is work thread
     * @_thread_id - thread id will be checked
     * @return - returns true if the thread is creater thread
    */
    bool m_isCreaterThread_(thread::id _thread_id);
    
    /* @m_isWorkerThread_ - check whether this thread is work thread
     * @_thread_id - thread id will be checked
     * @return - returns true if the thread is worker thread
    */
    bool m_isWorkerThread_(thread::id _thread_id);
    
    /* @m_threadChanged_() - check whether this object has been moved to another thread */
    bool m_threadChanged_();
    
    /* @m_threadSafe_() - check the thread status of this object and make sure this object is thread-safe
     * Returned B_ONLY_SINGLE_THREAD error code if this object is running in multi-thread mode.
    */
    int m_threadSafe_();
};

};

#endif
