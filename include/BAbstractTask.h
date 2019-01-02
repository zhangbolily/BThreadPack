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
    
    /* Make sure this task can only be accessed by one thread at any time */
    mutex m_task_mutex;
    
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
};

};

#endif
