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
    /* @BAbstractThreadPool() - Constructor
     * @_thread_num - How many thread you want to create.
    */
    BAbstractThreadPool(int _threadNum);
    
    /* @~BAbstractThreadPool() - Destructor
     * Don't need any parameter
    */
    ~BAbstractThreadPool();
    
    /* @getThreadNum - Get how many threads in this pool.
     * @return - return thread number.
    */
    unsigned int getThreadNum();
    
    /* @addThread - Add a thread to the thread pool.
     * @return - return current thread number.
    */
    int addThread(thread _newThread);
    
    /* @_initThreads_ - This function will initialize all threads.
     * @_this - Pass a fake this pointer into thread.
     * @return - return 0 if success
    */
    virtual int initThreads(BAbstractThreadPool* _this);
    
    /* @waitCond - This function wait the signal of condition_variable. 
     * No return value.
    */
    void waitCond();
    
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
