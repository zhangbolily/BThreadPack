#include "BAbstractTask.h"
#include <iostream>

using namespace std;
using namespace BThreadPack;

int main(int argc, char** argv)
{
    BAbstractTask _hello_task;
    _hello_task.setTaskData(1);
    
    {
        //Start the task.
        int _task_data_ = _hello_task.getTaskData();
        cout<<"The task data before porcessing is "<<_task_data_<<"."<<endl;
        
        _task_data_++;
        _hello_task.setTaskData(_task_data_);
        
        cout<<"The task data after porcessing is "<<_hello_task.getTaskData()<<"."<<endl;
    }

    cout<<"This will be a multi-thread hello world!"<<endl;

    return 0;
}
