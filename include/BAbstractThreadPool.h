/*
 * @Author      : Ball Chang
 * @File        : BAbstractThreadPool.h
 * @Date        : 2018-12-2
*/

#ifndef _BABSTRACT_THREAD_POOL_H_
#define _BABSTRACT_THREAD_POOL_H_

#include <thread>
#include <atomic>
#include <queue>
#include <vector>
#include <functional>
#include <condition_variable>
#include "BThreadPack.h"

using namespace std;

namespace BThreadPack {

class BAbstractThreadPool{

public:
	enum BThreadPoolStatus{
        ThreadPoolError = -1,
        ThreadPoolStop = 0,
        ThreadPoolRunning = 1,
    };
    
    enum BThreadControlMode{
        /* In this mode, thread capacity is fixed. */
        FixedThreadCapacity = 0,
        /* In this mode, thread capacity is dynamic controled.
         * If application doesn't specify the thread capacity, BAbstractThreadPool will caculate
         * a proper thread capacity baseed on the system hardware.
        */
        DynamicThreadCapacity = 1,
    };

    /* @BAbstractThreadPool() - Constructor
     * @_threadCap - The maximum number of threads this pool can create.
     * @_mode - This thread pool will work in which mode.Work mode is defined in BThreadControlMode.
    */
    BAbstractThreadPool(unsigned int _threadCap,
                        BAbstractThreadPool::BThreadControlMode _mode = BAbstractThreadPool::BThreadControlMode::FixedThreadCapacity);
    
    /* @~BAbstractThreadPool() - Destructor
     * Don't need any parameter
    */
    ~BAbstractThreadPool();
    
    /* @mode - The thread pool is running in which mode.
     * @return - return thread capacity number.
    */
    int mode();
    
    /* @capacity - Get how many threads can be stored in this pool.
     * @return - return thread capacity number.
    */
    unsigned int capacity();
    
    /* @setCapacity - Set how many threads can be stored in this pool.
     * If thread pool is running in DynamicThreadCapacity mode, the input _capacity is a reference value 
     * for calculating the best solution of capacity value.
     * @return - No return value.
    */
    void setCapacity(unsigned int _capacity);
    
    /* @size - Get how many threads in this pool.
     * @return - return thread size.
    */
    unsigned int size();
    
    /* @addThread - Add a thread to the thread pool.
     * You can add threads less than the thread pool capacity.
     * @return - return current thread number.
    */
    long long addThread(thread _newThread);
    
    /* @removeThread - Remove a thread from the thread pool.
     * You can remove the thread amount to 0.
     * @_threadNum - Which thread you want to delete.
     * @return - return none B_SUCCESS if error happens or return the current thread amount
     * with positive value.
    */
    long long removeThread(unsigned int _threadNum);
    
    /* @detach - This function will detach all threads.
     * @return - Return B_SUCCESS if operation success.
    */
    int detach();
    
    /* @detach - This function will detach the thread specified by _threadNum.
     * @_threadNum - Which thread you want to detach.The first thread number is 0.
     * @return - Return B_SUCCESS if operation success.
    */
    int detach(unsigned int _threadNum);
    
    /* @join - This function will join all threads.
     * @return - Return B_SUCCESS if operation success.
    */
    int join();
    
    /* @join - This function will join the thread specified by _threadNum.
     * @_threadNum - Which thread you want to join.The first thread number is 0.
     * @return - Return B_SUCCESS if operation success.
    */
    int join(unsigned int _threadNum);
    
    /* @wait - This function wait the signal of condition_variable. 
     * No return value.
    */
    void wait();
    
    /* @setStatus - Set the status of this thread pool.
     * @_status - New pool status value.
     * No return value.
    */
    void setStatus(BThreadPoolStatus _status);
    
    /* @status - Get the status of this thread pool.
     * @return the status of this thread pool.
    */
    int status();
    
    /* @startOneTask - This function will notify at least one thread to pop task queue.
     * If the task queue only has one task, this function behaves like start one task.
     * However, if the task queue has more than one task, the behaviour is undefined.
     * Don't need any parameters.
     * @return - return 0 if success
    */
    int startOneTask();
    
    /* @startOneTasks - This function will notify all threads to pop task queue.
     * @return - return 0 if success
    */
    int startAllTasks();

    /* @kill - Kill all threads and release all resources.
     * Don't need any parameters.
     * @return - return 0 if success
    */
    int kill();
    
    /* @addTask - Add a task to the task queue.
     * @_taskBuffer - A memory buffer that contains the task data.
     * @return - return 0 if success
    */
    int addTask(void* _taskBuffer);
    
    /* @getTask - Get a task from task queue.
     * @return - return task buffer.
    */
    void* getTask();

private:
    /* @_threadNum_ - The capacity of how many threads this pool can create.
    */
    atomic_uint _threadCap_;
    
    /* @_threadVec_ - The vector contains all threads of this pool.
    */
    vector<thread> _threadVec_;
    
    /* @_taskQueue_ - The queue of tasks.
    */
    queue<void *> _taskQueue_;
    
    /* @_poolMode_ - Control the running mode of this thread pool.
    */
    atomic_int _poolMode_;
    
    /* @_poolStatus_ - Control the status of this thread pool.
    */
    atomic_int _poolStatus_;
    
    /* @_taskQueueMut_ - Mutex for _taskQueue_.
    */
    mutex _taskMut_;
    
    /* @_startMut_ - Mutex for _startCond_.
    */
    mutex _startMut_;
    
    /* @_startCond_ - Determin whether start threads.
    */
    condition_variable _startCond_;
    
    /* @_init_ - This function will initialize all threads.
     * @_this - Pass a fake this pointer into thread.
     * All threads will use this pointer to call public member in this class.
     * Mostly for task management purpose.
     * @return - return 0 if success
    */
    virtual int _init_(BAbstractThreadPool* _this);

    /* @_threadFunction_ - The function that threads will execute.
     * @_buffer - Transfer data into thread.
    */
    static void _threadFunction_(void* _buffer);
};

};

#endif
