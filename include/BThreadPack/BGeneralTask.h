/*
 * @Author      : Ball Chang
 * @File        : BGeneralTask.h
 * @Date        : 2018-12-3
*/

#ifndef _BGENERAL_TASK_H_
#define _BGENERAL_TASK_H_

#include "BAbstractTask.h"

using namespace std;

namespace BThreadPack{

class BGeneralTask:public BAbstractTask{

public:
    /* @BGeneralTask() - Constructor */    
    BGeneralTask();
    BGeneralTask(bool _autodestroy);
    
    /* @BGeneralTask() - Constructor
     * @_buffer - the task data buffer
     * @_size - the size of buffer
    */    
    BGeneralTask(const void* _buffer, size_t _size);
    BGeneralTask(bool _autodestroy, const void* _buffer, size_t _size);
	
    /* @~BGeneralTask() - Destructor */
    virtual ~BGeneralTask();
    
    /* @execute() - This function will call _taskFunction to execute this task itself. */
    virtual int execute();
};

};

#endif