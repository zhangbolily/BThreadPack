/*
 * @Author      : Ball Chang
 * @File        : BWorkerTask.h
 * @Date        : 2018-12-2
*/

#ifndef _BWORKER_TASK_H_
#define _BWORKER_TASK_H_

#include "BAbstractTask.h"

using namespace std;

namespace BThreadPack{

class BWorkerTask:public BAbstractTask{

public:
    /* @BWorkerTask() - Constructor
     * Don't need any parameter
    */    
    BWorkerTask();
    
    /* @BWorkerTask() - Constructor
     * @_buffer - the task data buffer
     * @_size - the size of buffer
    */    
    BWorkerTask(void* _buffer, size_t _size);
	
    /* @~BWorkerTask() - Destructor
     * Don't need any parameter
    */
    ~BWorkerTask();
};

};
#endif
