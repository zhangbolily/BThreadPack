/*
 * @Author      : Ball Chang
 * @File        : BAbstractThreadPool.h
 * @Date        : 2018-12-2
*/

#ifndef _BABSTRACT_THREAD_POOL_H_
#define _BABSTRACT_THREAD_POOL_H_

#include <atomic>
#include <condition_variable>
#include <chrono>
#include <functional>
#include <map>
#include <queue>
#include <thread>
#include <vector>

#include "BThreadPack.h"

using namespace std;
using namespace std::chrono;

namespace BThreadPack {

class BAbstractThreadPool{

public:
	enum BThreadPoolStatus{
        ThreadPoolError = -1,
        ThreadPoolStop = 0,
        ThreadPoolRunning = 1
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
     * @_thread_cap - The maximum number of threads this pool can create.
    */
    BAbstractThreadPool(unsigned int _thread_cap);
    
    /* @BAbstractThreadPool() - Constructor
     * @_thread_cap - The maximum number of threads this pool can create.
     * @_mode - This thread pool will work in which mode.Work mode is defined in BThreadControlMode.
    */
    BAbstractThreadPool(unsigned int _thread_cap,
                        BAbstractThreadPool::BThreadControlMode _mode);
    
    /* @~BAbstractThreadPool() - Destructor */
    ~BAbstractThreadPool();
    
    /* @mode - The thread pool is running in which mode.
     * @return - returns pool mode.
    */
    int mode();
    
    /* @capacity - Get how many threads can be created in this pool.
     * @return - returns thread capacity.
    */
    unsigned int capacity();
    
    /* @setCapacity - Set how many threads can be stored in this pool.
     * If thread pool is running in DynamicThreadCapacity mode, the input _capacity will only
     * be a reference for calculating the best solution of capacity value.
    */
    void setCapacity(unsigned int _capacity);
    
    /* @size - Get how many threads in this pool. This size can be smaller than capacity.
     * @return - returns thread size.
    */
    unsigned int size();
    
    /* @addThread - Add a thread to the thread pool.
     * You can add threads less than the thread pool capacity.
     * @return - Returns current thread size. Less than 0 means error occured.
    */
    long long addThread(thread _new_thread);
    
    /* @removeThread - Remove a thread from the thread pool.
     * You can remove the thread number to 0.
     * @_thread_num - Which thread you want to delete.
     * @return - Returns none B_SUCCESS if error happens or return the current thread pool size
     * with positive value.
    */
    long long removeThread(unsigned int _thread_num);
    
    bool isRemove();
    
    void threadExit(std::thread::id _tid);
    
    bool isThreadExit(std::thread::id _tid);
    
    /* @detach - This function will detach all threads.
     * @return - Returns B_SUCCESS if operation success.
    */
    int detach();
    
    /* @detach - This function will detach the thread specified by _thread_num.
     * @_thread_num - Which thread you want to detach.The first thread number is 0.
     * @return - Returns B_SUCCESS if operation success.
    */
    int detach(unsigned int _thread_num);
    
    /* @join - This function will join all threads.
     * @return - Returns B_SUCCESS if operation success.
    */
    int join();
    
    /* @join - This function will join the thread specified by _thread_num.
     * @_thread_num - Which thread you want to join.The first thread number is 0.
     * @return - Returns B_SUCCESS if operation success.
    */
    int join(unsigned int _thread_num);
    
    /* @wait - This function waits the signal of condition_variable. */
    void wait();
    
    /* @setStatus - Set the status of this thread pool.
     * @_status - New pool status value.
    */
    void setStatus(BThreadPoolStatus _status);
    
    /* @status - Get the status of this thread pool.
     * @Returns the status of this thread pool.
    */
    int status();
    
    /* @startOneTask - This function will notify at least one thread to process task.
     * If the task queue only has one task, this function behaves like start one task.
     * However, if the task queue has more than one task, the behaviour is undefined.
     * @return - Returns B_SUCCESS if success
    */
    int startOneTask();
    
    /* @startOneTasks - This function will notify all threads to process task queue.
     * @return - Returns B_SUCCESS if success
    */
    int startAllTasks();

    /* @kill - Kill all threads and release all resources.
     * @return - Returns B_SUCCESS if success
    */
    int kill();
    
    /* @addTask - Add a task to the task queue.
     * @_task_buffer - A memory buffer that contains the task data.
     * @return - Returns B_SUCCESS if success
    */
    int addTask(void* _task_buffer);
    
    /* @getTask - Get a task from task queue.
     * @return - Returns task buffer.
    */
    void* getTask();
    
    /* @taskQueueSize - Get the size of task queue.
    */
    int taskQueueSize();
    
    /* @sendMessage - Send a message to the message queue and notify thread subscribing this message.
     * @_queue_num - You want to send message to which queue.
     * @_message_buffer - A memory buffer that contains the message data.
     * @return - Returns B_SUCCESS if success
    */
    int sendMessage(int _queue_num, void* _message_buffer);
    
    /* @message - Get a message from message queue. This function will be blocked until message arrived.
     * @_queue_num - Which bus you want to subscribe.
     * @return - Returns message buffer.
    */
    void* message(int _queue_num);

private:
    atomic_uint m_thread_capacity_;
    vector<thread> m_thread_vec_;
    vector<std::thread::id> m_thread_id_vec;
    queue<void *> m_task_queue_;
    atomic_int m_pool_mode_;
    atomic_int m_pool_status_;
    atomic_int m_remove_count;
    mutex m_task_mutex_;
    mutex m_message_mutex_;
    mutex m_start_mutex_;
    mutex m_thread_exit_mutex;
    condition_variable m_start_condition_;
    map<int, queue<void *>> m_message_queue_map_;
    map<int, condition_variable> m_message_cond_map_;
    map<int, mutex> m_message_mutex_map_;
    map<std::thread::id, bool> m_thread_exit_map;
    
    /* @_init_ - This function will initialize all threads.
     * @_this - Pass a fake this pointer into thread.
     * All threads will use this pointer to call public member in this class.
     * Mostly for task management purpose.
     * @return - return B_SUCCESS if success
    */
    virtual int _init_(BAbstractThreadPool* _this);

    /* @m_threadFunction_ - The function that threads will execute.
     * @_buffer - Transfer data into thread.
    */
    static void m_threadFunction_(void* _buffer);
};

};

#endif
