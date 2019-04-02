#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unistd.h>
#include <math.h>

#include "BThreadPack/BThreadPack.h"
#include "BThreadPack/BGeneralTask.h"
#include "BThreadPack/BGroupTask.h"
#include "BThreadPack/BThreadPool.h"

using namespace std;
using namespace BThreadPack;

const long long multiple = 4000000000000000000;

class CalculatePiTask: public BGeneralTask{
public:
    explicit CalculatePiTask(int _task_id)
        :BGeneralTask(false),
        m_task_id_(_task_id)
    {
    }

    ~CalculatePiTask() override = default;
    
    int execute() override
    {
#ifndef WIN32
        std::ostringstream _os;
        _os << "Pi slice #" << m_task_id_ << ": is calculating on CPU " << sched_getcpu() << "\n";
        std::cout << _os.str();
#endif
        long long _pi_slice_result = pi(m_task_id_);
        
        _os.str("");
        _os.clear();
        _os << "Pi slice #" << m_task_id_ << " result is " << _pi_slice_result << "\n";
        std::cout << _os.str();
        
        setOutputBuffer(&_pi_slice_result, sizeof(long long));
            
        return 0;
    }

private:
    const unsigned long long m_range = 40000000;
    int m_task_id_;
    
    long long pi(int _offset)
    {
        bool sign = true;
        long long _pi = 0;
        for(unsigned long long i = 0;i < m_range;i++)
        {
            if(sign)
                _pi += multiple/((i + _offset * m_range)*2 + 1);
            else
                _pi -= multiple/((i + _offset * m_range)*2 + 1);
            sign = !sign;
        }
        return _pi;
    }
};

int main(int argc, char** argv)
{
    //1. Initialize thread pool.
    uint num_threads = 8;
    BThreadPool pi_slice_pool(num_threads, BAbstractThreadPool::BThreadControlMode::DynamicThreadCapacity);

    int slice_num = 8;
    BGroupTask pi_group_task;

    //2. Add task into thread pool.
    for(int i = 0;i < slice_num;i++)
    {
        auto p_slice_pi = new CalculatePiTask(i);
        std::string _slice_name = "Pi slice #";
        _slice_name += std::to_string(i);
        p_slice_pi->setName(_slice_name);
        pi_group_task.pushTask(static_cast<BAbstractTask *>(p_slice_pi));
    }
    
    pi_slice_pool.pushGroupTask(&pi_group_task);
    
    cout << "Current threads: " << pi_slice_pool.size() <<endl;
    
    //3. Calculate pi
    long long ll_Pi = 0;
    double Pi = 0.0;
    
    pi_group_task.wait();   // Wait all tasks in this group finished.
    
    while(true){
        auto p_slice_pi = dynamic_cast<CalculatePiTask*>(pi_group_task.getFinishedTask());
        
        if(p_slice_pi == nullptr)
            break;
        
        const void* pi_slice_result = nullptr;
        size_t buffer_size;
        p_slice_pi->outputBuffer(&pi_slice_result, buffer_size);
        
        ll_Pi += *(static_cast<const long long *>(pi_slice_result));

        std::cout << "Task - " << p_slice_pi->name() << ":" << std::endl;
        std::cout << "UUID: " <<
                  p_slice_pi->UUID() << std::endl;
        std::cout << "Schedule time: " <<
                  p_slice_pi->realTime() - p_slice_pi->executionTime() << " us" << std::endl;
        std::cout << "Real time: " <<
                  p_slice_pi->realTime() << " us" << std::endl;
        std::cout << "Execution time: " <<
                  p_slice_pi->executionTime() << " us" << std::endl;
        
        delete p_slice_pi;
    }

    std::cout << "Group task Pi :" << std::endl;
    std::cout << "Schedule time: " <<
                pi_group_task.realTime() - pi_group_task.executionTime() << " us" << std::endl;
    std::cout << "Real time: " <<
                pi_group_task.realTime() << " us" << std::endl;
    std::cout << "Execution time: " <<
                pi_group_task.executionTime() << " us" << std::endl;
    
    std::cout.setf(ios::showpoint);
    std::cout.precision(64);
    
    std::cout << "Interger Pi is : " << ll_Pi << std::endl;
    Pi = ll_Pi/(multiple/4.0);
    std::cout << "Pi is : " << Pi << std::endl;
    
    pi_slice_pool.kill();
	
    return 0;
}
