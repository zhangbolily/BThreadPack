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
	enum class BThreadPoolStatus{
        ThreadPoolError = -1,
        ThreadPoolStop = 0,
        ThreadPoolRunning = 1,
    };
    
    enum class BThreadControlMode{
        /* In this mode, thread number is fixed. */
        FixedThreadNum = 0,
        /* In this mode, thread number is dynamic controled.
         * If application doesn't specify the thread number, BAbstractThreadPool will caculate
         * a proper thread number baseed on the system hardware.
        */
        DynamicThreadNum = 1,
    };

    /* @BAbstractThreadPool() - Constructor
     * @_thread_num - How many thread you want to create.
    */
    BAbstractThreadPool(int _threadNum, BAbstractThreadPool::BThreadControlMode _mode);
    
    /* @~BAbstractThreadPool() - Destructor
     * Don't need any parameter
    */
    ~BAbstractThreadPool();
    
    /* @getThreadCap - Get how many threads can be stored in this pool.
     * @return - return thread capacity value.
    */
    unsigned int getThreadCap();
    
    /* @getThreadNum - Get how many threads in this pool.
     * @return - return thread number.
    */
    unsigned int getThreadNum();
    
    /* @addThread - Add a thread to the thread pool.
     * You can add threads less than the thread pool capacity.
     * @return - return current thread number.
    */
    long long addThread(thread _newThread);
    
    /* @_initThreads_ - This function will initialize all threads.
     * @_this - Pass a fake this pointer into thread.
     * All threads will use this pointer to call public member in this class.
     * Mostly for task management purpose.
     * @return - return 0 if success
    */
    virtual int initThreads(BAbstractThreadPool* _this);
    
    /* @detachThreads - This function will detach all threads.
     * @return - No return value.
    */
    void detachThreads();
    
    /* @joinThreads - This function will join all threads.
     * @return - No return value.
    */
    void joinThreads();
    
    /* @waitCond - This function wait the signal of condition_variable. 
     * No return value.
    */
    void waitCond();
    
    /* @setThreadPoolStatus - Set the status of this thread pool.
     * No return value.
    */
    void setThreadPoolStatus(BThreadPoolStatus _status);
    
    /* @getThreadPoolStatus - Get the status of this thread pool.
     * @return the status of this thread pool.
    */
    BThreadPoolStatus getThreadPoolStatus();
    
    /* @startOneTask - This function will notify at least one thread to pop task queue.
     * If the task queue only has one task, this function behaves like start one task.
     * However, if the task queue has more than one task, the behaviour is undefined.
     * Don't need any parameters.
     * @return - return 0 if success
    */
    int startOneTask();
    
    /* @startOneTask - This function will notify all threads to pop task queue.
     * @return - return 0 if success
    */
    int startAllTask();

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
    /* @_threadNum_ - The number of threads.
    */
    atomic_uint _threadNum_;
    
    /* @_threadVec_ - The vector contains all threads of this pool.
    */
    vector<thread> _threadVec_;
    
    /* @_taskQueue_ - The queue of tasks.
    */
    queue<void *> _taskQueue_;
    
    /* @_poolStatus_ - Control the status of this thread pool.
    */
    BThreadPoolStatus _poolStatus_;
    
    /* @_taskQueueMut_ - Mutex for _taskQueue_.
    */
    mutex _taskMut_;
    
    /* @_startMut_ - Mutex for _startCond_.
    */
    mutex _startMut_;
    
    /* @_startCond_ - Determin whether start threads.
    */
    condition_variable _startCond_;

    /* @_threadFunction_ - The function that threads will execute.
     * @_buffer - Transfer data into thread.
    */
    static void _threadFunction_(void* _buffer);
};

};

#endif
