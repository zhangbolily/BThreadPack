#include "BThreadPack.h"
#include "task_data.pb.h"
#include <iostream>

using namespace std;
using namespace BThreadPack;

void FunHello(void* buffer)
{
    if(buffer == nullptr)
    {
        return;
    }
    
    BAbstractTask* p_hello_task = (BAbstractTask*)buffer;
    
    void* task_buffer;
    size_t task_buffer_size;
    
    if(p_hello_task->getInputBuffer(&task_buffer, task_buffer_size) == B_ONLY_SINGLE_THREAD)
    {
        cerr<<"[Error] Get task input buffer failed.\n";
        return;
    }
    
    TaskData task_data;
    task_data.ParseFromArray(task_buffer, task_buffer_size);
    
    cout<<task_data.message()<<endl;
}

int main(int argc, char** argv)
{
    size_t buffer_size = 0;
    void* task_buffer = nullptr;
    BAbstractTask hello_world_task;
    
    //1. Make task data
    
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
    
    if(hello_world_task.setInputBuffer(task_buffer, buffer_size))
    {
        cerr<<"[Error] hello_world_task.setInputBuffer failed."<<endl;
        return -1;
    }
    
    //2. Start threads
    int num_threads = 1;
    std::thread hello_thread[num_threads];
    
    for (int i = 0; i < num_threads; ++i) {
        hello_thread[i] = std::thread(FunHello, &hello_world_task);
    }
    
    for (int i = 0; i < num_threads; ++i) {
        hello_thread[i].join();
    }
    
    if(task_buffer != nullptr)
    {
        free(task_buffer);
        task_buffer = nullptr;
    }

    return 0;
}
