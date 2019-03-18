/*
 * @Author      : Ball Chang
 * @File        : BGeneralTask.h
 * @Date        : 2018-12-3
*/

#ifndef _BGENERAL_TASK_H_
#define _BGENERAL_TASK_H_

#include <string>

#include "BThreadPack/BAbstractTask.h"
#include "BThreadPack/BTimer.h"

namespace BThreadPack{

class BGeneralTask:public BAbstractTask{

public:
    /* @BGeneralTask() - Constructor */    
    BGeneralTask();
    BGeneralTask(bool _autodestroy);
    
    /* @~BGeneralTask() - Destructor */
    virtual ~BGeneralTask();
    
    /* @BGeneralTask() - Constructor
     * @_buffer - the task data buffer
     * @_size - the size of buffer
    */    
    BGeneralTask(const void* _buffer, size_t _size);
    BGeneralTask(bool _autodestroy, const void* _buffer, size_t _size);
    
    /* Time of execution*/
    void startExecutionTiming();
    void stopExecutionTiming();
    long long executionTime();
    
    /* Time between pushing to the task queue and finished execution*/
    void startRealTiming();
    void stopRealTiming();
    long long realTime();
    
    /* Set the task name. This name will be shown on thread name. */
    void setName(const char* _name);
    void setName(std::string _name);
    const std::string& name() const;
    
    /* @execute() - This function will call _taskFunction to execute this task itself. */
    virtual int execute() = 0;

private:
    BTimer m_real_timer;
    BTimer m_execute_timer;
    std::string m_name;
};

};

#endif
