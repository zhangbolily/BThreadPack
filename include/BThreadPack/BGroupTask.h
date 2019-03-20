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
 * @File        : BGroupTask.h
 * @Date        : 2019-3-17
*/

#ifndef _BGROUP_TASK_H_
#define _BGROUP_TASK_H_

#include <vector>

#include "BThreadPack/BGeneralTask.h"
#include "BThreadPack/BTimer.h"

namespace BThreadPack{

template<class TaskType>
class BGroupTask:private NoneCopy{
    typedef TaskType group_task_type;

public:
    /* @BGroupTask() - Constructor */    
    BGroupTask();
    ~BGroupTask();
    
    void pushTask(group_task_type* _task_handle)
    {
        m_task_vec.push_back(_task_handle);
    }
    
    int removeTask(unsigned int _task_id)
    {
        if(_task_id >= size())
            return ReturnCode::BError;
        else {
            typename std::vector<group_task_type*>::iterator it = m_task_vec.begin();
            m_task_vec.erase(it + _task_id);
            
            return ReturnCode::BSuccess;
        }
    }
    
    group_task_type* getTask(unsigned int _task_id)
    {
        if(_task_id >= size())
            return nullptr;
        else {
            return m_task_vec[m_task_vec];
        }
    }
    
    unsigned int size()
    {
        return m_task_vec.size();
    }
    
    /* Time of execution*/
    void startExecutionTiming()
    {
        m_execute_timer.start();
    }

    void stopExecutionTiming()
    {
        m_execute_timer.stop();
    }

    long long executionTime()
    {
        return m_execute_timer.time();
    }
    
    /* Time between pushing to the task queue and finished execution*/
    void startRealTiming()
    {
        m_real_timer.start();
    }

    void stopRealTiming()
    {
         m_real_timer.stop();
    }

    long long realTime()
    {
        return m_real_timer.time();
    }
    
private:
    std::vector<group_task_type*> m_task_vec;
    BTimer m_real_timer;
    BTimer m_execute_timer;
};
};

#endif
