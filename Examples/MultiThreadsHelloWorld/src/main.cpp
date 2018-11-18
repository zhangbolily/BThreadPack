#include "BThreadPack.h"
#include "task_data.pb.h"
#include <iostream>
#include <queue>
#include <unistd.h>
#include <mutex>
#include <condition_variable>

using namespace std;
using namespace BThreadPack;

mutex task_mut;
condition_variable task_cond;
queue<BAbstractTask*> q_hello_world;

void FunHello()
{
    while(1){
        std::unique_lock<std::mutex> lock(task_mut);
        task_cond.wait(lock);
        
        BAbstractTask* p_hello_task = (BAbstractTask*)q_hello_world.front();
        
        void* task_buffer;
        size_t task_buffer_size;
        
        if(p_hello_task->getInputBuffer(&task_buffer, task_buffer_size) == B_ONLY_SINGLE_THREAD)
        {
            cerr<<"[Error] Get task input buffer failed.\n";
            lock.unlock();
            return;
        }
        
        q_hello_world.pop();
        
        lock.unlock();
        
        TaskData task_data;
        task_data.ParseFromArray(task_buffer, task_buffer_size);
        
        cout<<"\033[37;42m"<<this_thread::get_id()<<" Task id is:"<<task_data.taskid()<<"\033[0m"<<endl;
        cout<<"\033[37;42m"<<this_thread::get_id()<<" Task message is:"<<task_data.message()<<"\033[0m"<<endl;
        
        if(task_buffer != nullptr)
        {
            free(task_buffer);
            task_buffer = nullptr;
        }
        
        if(q_hello_world.size())
            task_cond.notify_all();
    }
}

int main(int argc, char** argv)
{
    int task_num = 20;
    
    //1. Start threads
    int num_threads = 10;
    std::thread hello_thread[num_threads];
    
    for (int i = 0; i < num_threads; ++i) {
        hello_thread[i] = std::thread(FunHello);
    }
    
    //2. Make task data
    for(int i=0;i < task_num;i++)
    {
        size_t buffer_size = 0;
        void* task_buffer = nullptr;
        BAbstractTask* hello_world_task = new BAbstractTask();
        
        TaskData task_data;
        task_data.set_message("This will be a multi-thread hello world!");
        task_data.set_taskid(i);
        
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
        
        if(hello_world_task->setInputBuffer(task_buffer, buffer_size))
        {
            cerr<<"[Error] hello_world_task.setInputBuffer failed."<<endl;
            return -1;
        }
        
        q_hello_world.push(hello_world_task);
    }      
        
    sleep(1);
    task_cond.notify_all();
    
    for (int i = 0; i < num_threads; ++i) {
        hello_thread[i].join();
    }

    return 0;
}
