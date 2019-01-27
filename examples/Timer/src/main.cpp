#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <setjmp.h>
#include <sys/syscall.h>

#include "BThreadPack/BThreadPack.h"
#include "BThreadPack/BTimer.h"
#include "BThreadPack/BGeneralTask.h"
#include "BThreadPack/BGeneralThreadPool.h"

using namespace std;
using namespace BThreadPack;

void jumpAction(int signum, siginfo_t* _sig, void* context);

class SignalTask: public BGeneralTask{
public:
    virtual int process()
    {
	    BTimer timer;
	    jmp_buf context_buf;
	    cout<<std::this_thread::get_id()<<endl;
	    cout<<syscall(__NR_gettid)<<endl;
	    
	    if(setjmp(context_buf) == 0){
	        if (timer.callOnTimeout(jumpAction, static_cast<void*>(&context_buf), syscall(__NR_gettid)) != ReturnCode::BSuccess)
	        {
	            fprintf(stderr, "callOnTimeout failed! Error with msg is: %s\n",strerror(errno));
	            return -1;
	        }
	        timer.setInterval(0);
	        if (timer.start(1000) != ReturnCode::BSuccess)
	        {
	            fprintf(stderr, "Start timer failed! Error with msg is: %s\n",strerror(errno));
	            return -1;
	        }
	        pause();
	    }
	    
	    cout<<"Don't jump, this thread will exit."<<endl;
	    
        return 0;
    }
};

void print(sigval _sig)
{
    cout<<"This is timer."<<endl;
    cout<<static_cast<char*>(_sig.sival_ptr)<<endl;
}

void printAction(int signum, siginfo_t* _sig, void* context)
{
    cout<<"This is timer."<<endl;
    cout<<getpid()<<endl;
    cout<<static_cast<char*>(_sig->_sifields._rt.si_sigval.sival_ptr)<<" -1"<<endl;
    cout<<static_cast<char*>(_sig->si_ptr)<<" -2"<<endl;
}

void jumpAction(int signum, siginfo_t* _sig, void* context)
{
    cout<<"This is jumper."<<endl;
    cout<<syscall(__NR_gettid)<<endl;
    longjmp(*(static_cast<jmp_buf*>(_sig->si_ptr)), 1);
}

int num_threads = 6;
BGeneralThreadPool signal_task_pool(num_threads);

int main(int argc, char** argv)
{
    if(signal_task_pool.removeThread(2) == ReturnCode::BThreadNotExists)
    {
        cout<<"Try to remove a none exist thread."<<endl;
    }
    cout<<signal_task_pool.size()<<endl;
    
    int task_num = 4;
    for(int i=0;i < task_num;i++)
    {
        SignalTask* _task = new SignalTask();
        signal_task_pool.addTask(_task);
    }
    
    signal_task_pool.startAllTasks();
    
    getchar();
    
    signal_task_pool.kill();
	
    return 0;
}
