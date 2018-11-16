/*
 * @Author      : Ball Chang
 * @File        : BAbstractTask.h
 * @Date        : 2018-11-12
*/

#ifndef _BABSTRACT_TASK_H_
#define _BABSTRACT_TASK_H_

#include <atomic>

using namespace std;

namespace BThreadPack {

class BAbstractTask{

private:

    /* @_taskStatus_ - store the status of task
     *
    */
    atomic_int _taskStatus_;
    
    /* @_taskStatus_ - store the data of task
     *
    */
    int _taskData_;

public:
    enum class BTaskStatus{
        TaskFailed = -1,
        TaskInit = 0,
        TaskDoing = 1,
        TaskFinished = 2
    };

    /* @BAbstractTask() - Constructor
     * Don't need any parameters
    */    
    BAbstractTask();
	
    /* @~BAbstractTask() - Destructor
     * Don't need any parameters
    */
    ~BAbstractTask();

    /* @getTaskStatus() - Return the _taskStatus_ value
     * Don't need any parameters
    */
    int getTaskStatus() const;

    /* @setTaskStatus() - set the _taskStatus_ value
     * @parameter status : the new value of _taskStatus_
    */
    void setTaskStatus(BTaskStatus _status);
    
    /* @setTaskStatus() - set the _taskStatus_ value
     * @parameter status : the new value of _taskStatus_ with type atomic_int
    */
    void setTaskStatus(atomic_int _status);
    
    /* @getTaskData() - Return the _taskData_ value
     * Don't need any parameters
    */
    int getTaskData() const;
    
    /* @setTaskData() - set the _taskData_ value
     * @_data - the task data
    */
    void setTaskData(int _data);

};

};

#endif
