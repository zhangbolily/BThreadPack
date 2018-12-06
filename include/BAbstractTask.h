/*
 * @Author      : Ball Chang
 * @File        : BAbstractTask.h
 * @Date        : 2018-11-12
*/

#ifndef _BABSTRACT_TASK_H_
#define _BABSTRACT_TASK_H_

#include <atomic>
#include <map>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <iostream>

using namespace std;

namespace BThreadPack {

class BAbstractTask{

public:
    enum class BTaskStatus{
        TaskFailed = -1,
        TaskInit = 0,
        TaskProcessing = 1,
        TaskFinished = 2
    };

    /* @BAbstractTask() - Constructor
     * Don't need any parameter
    */    
    BAbstractTask();
    
    /* @BAbstractTask() - Constructor
     * @_buffer - the task data buffer
     * @_size - the size of buffer
    */    
    BAbstractTask(void* _buffer, size_t _size);
	
    /* @~BAbstractTask() - Destructor
     * Don't need any parameter
    */
    ~BAbstractTask();

    /* @status() - Return the _taskStatus_ value
     * Don't need any parameter
    */
    int status();

    /* @setStatus() - set the _taskStatus_ value
     * @parameter status - the new value of _taskStatus_
    */
    int setStatus(BTaskStatus _status);
    
    /* @setStatus() - set the _taskStatus_ value
     * @parameter status - the new value of _taskStatus_ with type atomic_int
    */
    int setStatus(atomic_int _status);
    
    /* @setInputBufer() - set the _inputBuffer_ value
     * @_buffer - the task input buffer
     * @_size - the size of buffer
     * @return - return 0 if success
    */
    int setInputBuffer(void* _buffer, size_t _size);
    
    /* @inputBuffer() - get the _inputBuffer_ value
     * @_buffer - the task output buffer
     * @_size - the size of buffer
    */
    int inputBuffer(void** _buffer, size_t &_size);
    
    /* @setOutputBufer() - set the _outputBuffer_ value
     * @_buffer - the task output buffer
     * @_size - the size of buffer
     * @return - return 0 if success
    */
    int setOutputBuffer(void* _buffer, size_t _size);
    
    /* @outputBuffer() - get the _outputBuffer_ value
     * @_buffer - the task output buffer
     * @_size - the size of buffer
    */
    int outputBuffer(void** _buffer, size_t &_size);
    
private:

    /* @_taskStatus_ - store the status of task
     *
    */
    atomic_int _taskStatus_;
    
    /* @_inputBuffer - store the buffer of input data
     *
    */
    void* _inputBuffer_;
    
    /* @_inputBufferSize - store the size of input data buffer
     *
    */
    atomic_size_t _inputBufferSize_;
    
    /* @_outputBuffer_ - store the buffer of output data
     *
    */
    void* _outputBuffer_;
    
    /* @_outputBuffer_ - store the size of output data buffer
     *
    */
    atomic_size_t _outputBufferSize_;
    
    /* @_threadRefCount_ - store how many threads are using this object
     *
    */
    atomic_int _threadRefCount_;
    
    /* @_createThreadID_ - store which thread create this object
     *
    */
    thread::id _createThreadID_;
    
    /* @_workThreadID_ - store which thread use this object
     *
    */
    thread::id _workThreadID_;
    
    /* @_isCreateThread_ - check whether this thread is work thread
     * @_threadID - thread id will be checked
    */
    bool _isCreateThread_(thread::id _threadID);
    
    /* @_isWorkThread_ - check whether this thread is work thread
     * @_threadID - thread id will be checked
    */
    bool _isWorkThread_(thread::id _threadID);
    
    /* @_threadChanged() - check whether this object has beed moved to another thread
     * Don't need any parameterr
    */
    bool _threadChanged_();
    
    /* @_threadSafe() - check the thread status of this object and make sure this object is thread-safe
     * Don't need any parameterr
     * Returned B_ONLY_SINGLE_THREAD error code if this object is running in multi-thread mode.
    */
    int _threadSafe_();

};

};

#endif
