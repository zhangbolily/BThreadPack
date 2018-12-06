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
    /* @BGeneralTask() - Constructor
     * Don't need any parameter
    */    
    BGeneralTask();
    
    /* @BGeneralTask() - Constructor
     * @_buffer - the task data buffer
     * @_size - the size of buffer
    */    
    BGeneralTask(void* _buffer, size_t _size);
	
    /* @~BGeneralTask() - Destructor
     * Don't need any parameter
    */
    ~BGeneralTask();
    
    /* @process() - This function will call _taskFunction to process this task itself.
     * No return value.
    */
    virtual int process();
};

};

#endif
