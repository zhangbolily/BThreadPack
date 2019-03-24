/* MIT License
* 
* Copyright (c) 2018 Ball Chang
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

/*
 * @Author      : Ball Chang
 * @File        : BGeneralThreadPool.h
 * @Date        : 2018-12-3
*/

#ifndef _BGENERAL_THREAD_POOL_H_
#define _BGENERAL_THREAD_POOL_H_

#define TASK_TIME_QUEUE	0

#ifdef WIN32
#include <windows.h>
#else
#include <sys/prctl.h>
#endif

#include <string>

#include "BUtils/BTimer.h"
#include "BThreadPack/BThreadPack.h"
#include "BThreadPack/BAbstractThreadPool.h"
#include "BThreadPack/BGeneralTask.h"

using namespace std;

namespace BThreadPack {

class BAbstractThreadPoolPrivate;

class BGeneralThreadPool: public BAbstractThreadPool{

public:
	enum Optimizer {
	    None = 0,
		PerformanceFirst = 1,
		ProcessTimeFirst = 2
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
    
    /* Overload pushTask */
    void pushTask(BGeneralTask* _task_ptr);
    void pushGroupTask(BGroupTask* _task_ptr);
    
    /* @optimizer() - Find the minimum threads for maximum thread pool performance
     * and the maximum threads for minimum single task processing time.
     * Call this function need thread pool running in DynamicThreadCapacity mode.
     * @_op_type - Which optimizer you want to use.
     * @_task_vec - The task for testing.
     */
    int optimizer(vector<BGeneralTask *> _task_vec, BGeneralThreadPool::Optimizer _op_type);
    
    virtual unsigned int resize(unsigned int _size);
    
    void setOptimizePolicy(BGeneralThreadPool::Optimizer _policy);
    const BGeneralThreadPool::Optimizer optimizePolicy() const;
    
    vector<unsigned long long> m_task_time_vec;
    
private:
    unsigned int m_max_performance_threads_;
    unsigned int m_min_time_threads_;
    BGeneralThreadPool::Optimizer m_optimize_policy;

	/* @_init_ - This function will initialize all threads.
     * @_this - Pass a fake this pointer into thread.
     * All threads will use this pointer to call public member in this class.
     * Mostly for task management purpose.
     * @return - return B_SUCCESS if success
    */
    virtual int m_init_(BGeneralThreadPool* _thread_pool_handle);
    virtual int m_init_(BGeneralThreadPool* _thread_pool_handle, unsigned int _thread_num);
    int m_normalOptimizer_(vector<BGeneralTask *> _task_vec);
    
    static void m_threadFunction_(BGeneralThreadPool* _thread_pool_handle);
    static void m_setThreadName(const char* _name);
    static void m_setThreadName(const std::string _name);
};

};

#endif
