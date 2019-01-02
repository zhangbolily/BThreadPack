/*
 * @Author      : Ball Chang
 * @File        : BGeneralThreadPool.h
 * @Date        : 2018-12-3
*/

#ifndef _BGENERAL_THREAD_POOL_H_
#define _BGENERAL_THREAD_POOL_H_

#define TASK_TIME_QUEUE	0

#include "BAbstractThreadPool.h"
#include "BGeneralTask.h"
#include "BTimer.h"

using namespace std;

namespace BThreadPack {

class BGeneralThreadPool: public BAbstractThreadPool{

public:
	enum Optimizer {
		PerformanceFirst = 0,
		ProcessTimeFirst = 1
	};
    /* @BAbstractThreadPool() - Constructor
     * @_thread_cap - How many threads can be created in this pool.
    */
    BGeneralThreadPool(unsigned int _thread_cap);
    
    /* @BAbstractThreadPool() - Constructor
     * @_thread_cap - How many threads can be created in this pool.
     * @_mode - This thread pool will work in which mode.Work mode is defined in BThreadControlMode.
    */
    BGeneralThreadPool(unsigned int _thread_cap,
                        BAbstractThreadPool::BThreadControlMode _mode);
    
    /* @~BAbstractThreadPool() - Destructor */
    ~BGeneralThreadPool();
    
    /* @optimizer() - Find the minimum threads for maximum thread pool performance
     * and the maximum threads for minimum single task processing time.
     * Call this function need thread pool running in DynamicThreadCapacity mode.
     * @_op_type - Which optimizer you want to use.
     * @_task_vec - The task for testing.
     */
    int optimizer(vector<BGeneralTask *> _task_vec, Optimizer _op_type);
    
    vector<unsigned long long> m_task_time_vec;
    
private:
    unsigned int m_max_performance_threads_;
    unsigned int m_min_time_threads_;

	/* @_init_ - This function will initialize all threads.
     * @_this - Pass a fake this pointer into thread.
     * All threads will use this pointer to call public member in this class.
     * Mostly for task management purpose.
     * @return - return B_SUCCESS if success
    */
    virtual int m_init_(BGeneralThreadPool* _this);
    virtual int m_init_(BGeneralThreadPool* _this, unsigned int _thread_num);
    static void m_threadFunction_(BGeneralThreadPool* _this);
    int m_normalOptimizer_(vector<BGeneralTask *> _task_vec);
};

};

#endif
