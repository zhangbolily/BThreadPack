#include "BThreadPack/BThreadPack.h"
#include "BThreadPack/BWorkerTask.h"
#include "task_data.pb.h"
#include <iostream>
#include <sstream>
#include <queue>
#include <unistd.h>
#include <mutex>
#include <condition_variable>

using namespace std;
using namespace BThreadPack;

mutex task_mut;
condition_variable task_cond;
queue<BWorkerTask*> q_hello_world;

void FunHello()
{
    while(1){
        std::unique_lock<std::mutex> lock(task_mut);
        task_cond.wait(lock);
        
        if(!q_hello_world.size())
        {
            task_cond.notify_all();
            return;
        }
        
        BWorkerTask* p_hello_task = (BWorkerTask*)q_hello_world.front();
        
        void* task_buffer;
        size_t task_buffer_size;
        std::ostringstream _os;
        
        if(p_hello_task->getInputBuffer(&task_buffer, task_buffer_size) == B_ONLY_SINGLE_THREAD)
        {
            _os<<"[Error] Get task input buffer failed.\n";
            cerr<<_os.str();
            lock.unlock();
            return;
        }
        
        q_hello_world.pop();
        
        lock.unlock();
        
        TaskData task_data;
        task_data.ParseFromArray(task_buffer, task_buffer_size);
        
        _os<<"\033[32m"<<"Task id is:"<<task_data.taskid()<<"\033[0m - ";
        _os<<"\033[32m"<<task_data.message()<<"\033[0m\n";
        cout<<_os.str();
        
        if(task_buffer != nullptr)
        {
            free(task_buffer);
            task_buffer = nullptr;
        }
        
        if(p_hello_task != nullptr)
        {
            delete p_hello_task;
            p_hello_task = nullptr;
        }
        
        task_cond.notify_all();
    }
}

int main(int argc, char** argv)
{
    int task_num = 100;
    
    //1. Start threads
    int num_threads = 20;
    std::thread hello_thread[num_threads];
    
    for (int i = 0; i < num_threads; ++i) {
        hello_thread[i] = std::thread(FunHello);
    }
    
    //2. Make task data
    for(int i=0;i < task_num;i++)
    {
        size_t buffer_size = 0;
        void* task_buffer = nullptr;
        BWorkerTask* hello_world_task = new BWorkerTask();
        
        TaskData task_data;
        task_data.set_message("This is a multi-thread hello world!");
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
