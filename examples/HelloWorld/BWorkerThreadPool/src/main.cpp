#include <iostream>
#include <sstream>
#include <queue>
#include <string>
#include <unistd.h>
#include <mutex>
#include <condition_variable>

#include "BThreadPack/BThreadPack.h"
#include "BThreadPack/BWorkerTask.h"
#include "BThreadPack/BAbstractThreadPool.h"

using namespace std;
using namespace BThreadPack;

const int LANGUAGES = 9;
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
    HelloWorldThreadPool(int _thread_num)
        :BAbstractThreadPool(_thread_num)
    {
    	HelloWorldThreadPool::initThreads(static_cast<BAbstractThreadPool*>(this));
    }
    
    ~HelloWorldThreadPool()
    {
    }
    
    int initThreads(BAbstractThreadPool* _this)
    {    
        for (unsigned int i = 0; i < this->capacity(); ++i) {
            if(this->addThread(thread(HelloWorldThreadPool::_threadFunction_, _this)) == BCore::ReturnCode::BThreadPoolFull)
                return BCore::ReturnCode::BThreadPoolFull;
        }
        
        return 0;
    }
    
    unsigned int resize(unsigned int _size)
    {
        return 0;
    }

private:
    static void _threadFunction_(BAbstractThreadPool* _this)
    {
        while(1)
        {
            _this->wait();
            
            BWorkerTask* p_hello_task = static_cast<BWorkerTask*>(_this->getTask());
            
            if(p_hello_task == nullptr)
                return;
            
            void* task_buffer;
            size_t task_buffer_size;
            std::ostringstream _os;
            
            if(p_hello_task->inputBuffer(&task_buffer, task_buffer_size) == BCore::ReturnCode::BOnlySingleThread)
            {
                _os << "[Error] Get task input buffer failed.\n";
                std::cerr << _os.str();
                return;
            }
            
            std::string language_str(static_cast<const char *>(task_buffer));
#ifdef WIN32
            if(language_str == language[1])
            {
            	_os << "Hello World!\n";
		        std::cout << _os.str();
            }else if(language_str == language[2])
            {
            	_os << "Bonjour le monde!\n";
		        std::cout << _os.str();
            }else if(language_str == language[3])
            {
            	_os << "Hola Mundo!\n";
		        std::cout << _os.str();
            }else {
                _os << "Sorry, don't support this language on Windows.'\n";
		        std::cout << _os.str();
            }
#else
            if(language_str == language[0])
            {
            	_os << "\033[32m" << "你好，世界！" << "\033[0m\n";
		        std::cout << _os.str();
            }else if(language_str == language[1])
            {
            	_os << "\033[32m" << "Hello World!" << "\033[0m\n";
		        std::cout << _os.str();
            }else if(language_str == language[2])
            {
            	_os << "\033[32m" << "Bonjour le monde!" << "\033[0m\n";
		        std::cout << _os.str();
            }else if(language_str == language[3])
            {
            	_os << "\033[32m" << "Hola Mundo!" << "\033[0m\n";
		        std::cout << _os.str();
            }else if(language_str == language[4])
            {
            	_os << "\033[32m" << "नमस्ते दुनिया!" << "\033[0m\n";
		        std::cout << _os.str();
            }else if(language_str == language[5])
            {
            	_os << "\033[32m" << "Привет, мир!" << "\033[0m\n";
		        std::cout << _os.str();
            }else if(language_str == language[6])
            {
            	_os << "\033[32m" << "こんにちは世界" << "\033[0m\n";
		        std::cout << _os.str();
            }else if(language_str == language[7])
            {
            	_os << "\033[32m" << "안녕하세요!" << "\033[0m\n";
		        std::cout << _os.str();
            }else if(language_str == language[8])
            {
            	_os << "\033[32m" << "🙋  🌎" << "\033[0m\n";
		        std::cout << _os.str();
            }
#endif
            
            if(p_hello_task != nullptr)
            {
                delete p_hello_task;
                p_hello_task = nullptr;
            }
            
            _this->startAllTasks();
        }
    }
};

int num_threads = 12;
HelloWorldThreadPool hello_world_pool(num_threads);

int main(int argc, char** argv)
{   
    //1. Make task data
    for(int i=0;i < LANGUAGES;i++)
    {
        BWorkerTask* hello_world_task = new BWorkerTask();
        
        if(hello_world_task->setInputBuffer(language[i], strlen(language[i]) + 1))
        {
            cerr<<"[Error] hello_world_task.setInputBuffer failed."<<endl;
            return -1;
        }

        hello_world_pool.pushTask(static_cast<BAbstractTask*>(hello_world_task));
    }      
    
    hello_world_pool.detach();
    hello_world_pool.startAllTasks();
    
    getchar();
    
    hello_world_pool.kill();

    return 0;
}
