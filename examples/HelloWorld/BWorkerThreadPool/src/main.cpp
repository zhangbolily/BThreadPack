#include "BThreadPack/BThreadPack.h"
#include "BThreadPack/BWorkerTask.h"
#include "BThreadPack/BAbstractThreadPool.h"
#include "task_data.pb.h"
#include <iostream>
#include <sstream>
#include <queue>
#include <unistd.h>
#include <mutex>
#include <condition_variable>

using namespace std;
using namespace BThreadPack;

#define LANGUAGES 9
const char* language[LANGUAGES] = {"Chinese",
									"English",
									"French",
									"Spanish",
									"Indian",
									"Russian",
									"Japanese",
									"Korean",
									"Emoji"
									};

class HelloWorldThreadPool: public BAbstractThreadPool{
public:
    HelloWorldThreadPool(int _threadNum)
        :BAbstractThreadPool(_threadNum)
    {
    	this->initThreads(this);
    }
    
    ~HelloWorldThreadPool()
    {
    }
    
    int initThreads(BAbstractThreadPool* _this)
    {    
        for (unsigned int i = 0; i < this->capacity(); ++i) {
            if(this->addThread(thread(HelloWorldThreadPool::_threadFunction_, _this)) == B_THREAD_POOL_IS_FULL)
                return B_THREAD_POOL_IS_FULL;
        }
        
        return 0;
    }

private:
    static void _threadFunction_(BAbstractThreadPool* _this)
    {
        while(1)
        {
            _this->wait();
            
            BWorkerTask* p_hello_task = (BWorkerTask*)_this->getTask();
            
            if(p_hello_task == nullptr)
                return;
            
            void* task_buffer;
            size_t task_buffer_size;
            std::ostringstream _os;
            
            if(p_hello_task->inputBuffer(&task_buffer, task_buffer_size) == B_ONLY_SINGLE_THREAD)
            {
                _os<<"[Error] Get task input buffer failed.\n";
                cerr<<_os.str();
                return;
            }
            
            TaskData task_data;
            task_data.ParseFromArray(task_buffer, task_buffer_size);
            
            if(task_data.language() == language[0])
            {
            	_os<<"\033[32m"<<"你好，世界！"<<"\033[0m\n";
		        cout<<_os.str();
            }else if(task_data.language() == language[1])
            {
            	_os<<"\033[32m"<<"Hello World!"<<"\033[0m\n";
		        cout<<_os.str();
            }else if(task_data.language() == language[2])
            {
            	_os<<"\033[32m"<<"Bonjour le monde!"<<"\033[0m\n";
		        cout<<_os.str();
            }else if(task_data.language() == language[3])
            {
            	_os<<"\033[32m"<<"Hola Mundo!"<<"\033[0m\n";
		        cout<<_os.str();
            }else if(task_data.language() == language[4])
            {
            	_os<<"\033[32m"<<"नमस्ते दुनिया!"<<"\033[0m\n";
		        cout<<_os.str();
            }else if(task_data.language() == language[5])
            {
            	_os<<"\033[32m"<<"Привет, мир!"<<"\033[0m\n";
		        cout<<_os.str();
            }else if(task_data.language() == language[6])
            {
            	_os<<"\033[32m"<<"こんにちは世界"<<"\033[0m\n";
		        cout<<_os.str();
            }else if(task_data.language() == language[7])
            {
            	_os<<"\033[32m"<<"안녕하세요!"<<"\033[0m\n";
		        cout<<_os.str();
            }else if(task_data.language() == language[8])
            {
            	_os<<"\033[32m"<<"🙋  🌎"<<"\033[0m\n";
		        cout<<_os.str();
            }
            
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
            
            _this->startAllTasks();
        }
    }
};

int num_threads = 20;
HelloWorldThreadPool hello_world_pool(num_threads);

int main(int argc, char** argv)
{   
    //1. Make task data
    for(int i=0;i < LANGUAGES;i++)
    {
        size_t buffer_size = 0;
        void* task_buffer = nullptr;
        BWorkerTask* hello_world_task = new BWorkerTask();
        
        TaskData task_data;
        task_data.set_language(language[i]);
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
        
        hello_world_pool.addTask((void *)hello_world_task);
    }      
    
    hello_world_pool.detach();
    hello_world_pool.startAllTasks();
    
    getchar();
    
    hello_world_pool.kill();

    return 0;
}
