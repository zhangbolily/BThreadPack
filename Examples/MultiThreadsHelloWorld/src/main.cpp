#include "BThreadPack.h"
#include "task_data.pb.h"
#include <iostream>

using namespace std;
using namespace BThreadPack;

void FunHello(void* buffer, size_t size)
{
    if(buffer == nullptr)
    {
        return;
    }
    
    TaskData task_data;
    task_data.ParseFromArray(buffer, size);
    
    cout<<task_data.message()<<endl;
}

int main(int argc, char** argv)
{
    int buffer_size = 0;
    void* task_buffer = nullptr;
    BAbstractTask _hello_task;
    
    TaskData task_data;
    task_data.set_message("This will be a multi-thread hello world!");
    
    buffer_size = task_data.ByteSizeLong();
    task_buffer = (void*)malloc(buffer_size);
    
    if(task_buffer == nullptr)
    {
        cerr<<"[Error]task_buffer memory allocated failed."<<endl;
        return -1;
    }
    
    if(!task_data.SerializeToArray(task_buffer, buffer_size))
    {
        cerr<<"[Error] task_data SerializeToArray failed."<<endl;
        return -1;
    }
    
    _hello_task.setInputBuffer(task_buffer, buffer_size);
    
    FunHello(task_buffer, buffer_size);
    
    if(task_buffer != nullptr)
    {
        free(task_buffer);
        task_buffer = nullptr;
    }

    return 0;
}
