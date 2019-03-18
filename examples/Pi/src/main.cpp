#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unistd.h>
#include <math.h>

#include "BThreadPack/BThreadPack.h"
#include "BThreadPack/BGeneralTask.h"
#include "BThreadPack/BGeneralThreadPool.h"

#define PRECISION 2000

using namespace std;
using namespace BThreadPack;

const double multiple = 4000000000000.0;
std::condition_variable g_result_cond;
std::mutex g_result_mutex;
std::vector<double> g_pi_slice_result;

class CalculatePiTask: public BGeneralTask{
public:
    CalculatePiTask(int _task_id)
        :BGeneralTask(false),
        m_task_id_(_task_id)
    {
    }
    
    ~CalculatePiTask()
    {
    }
    
    virtual int execute()
    {
        std::ostringstream _os;
        _os << "Pi slice #" << m_task_id_ << ": is calculating on CPU " << sched_getcpu() << "\n";
        std::cout << _os.str();
        double _pi_slice_result = pi(m_task_id_);
        
        if(g_result_mutex.try_lock())
        {
        	if(g_pi_slice_result.size() > m_task_id_){
                g_pi_slice_result[m_task_id_] = _pi_slice_result;
                g_result_mutex.unlock();
                g_result_cond.notify_all();
                return 0;
            } else {
                g_pi_slice_result.push_back(_pi_slice_result);
                g_result_mutex.unlock();
                g_result_cond.notify_all();
                return 0;
            }
        }
        
        while(1)
        {
            std::unique_lock<std::mutex> lock(g_result_mutex);
            g_result_cond.wait(lock);
            if(g_result_mutex.try_lock())
            {
            	if(g_pi_slice_result.size() > m_task_id_){
                    g_pi_slice_result[m_task_id_] = _pi_slice_result;
                    g_result_mutex.unlock();
                    g_result_cond.notify_all();
                    return 0;
                } else {
                    g_pi_slice_result.push_back(_pi_slice_result);
                    g_result_mutex.unlock();
                    g_result_cond.notify_all();
                    return 0;
                }
            }
        }
            
        return 0;
    }

private:
    const unsigned long long m_range = 400000000;
    int m_task_id_;
    
    double pi(int _offset)
    {
        _offset;
        bool sign = true;
        double _pi = 0.0;
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

int num_threads = 8;
BGeneralThreadPool pi_slice_pool(num_threads, BAbstractThreadPool::BThreadControlMode::DynamicThreadCapacity);

int main(int argc, char** argv)
{   
    //vector<BGeneralTask *> task_vec;
    int slice_num = 24;
    //1. Add task into thread pool.
    for(int i = 0;i < slice_num;i++)
    {
        CalculatePiTask* p_slice_pi = new CalculatePiTask(i);
        std::string _slice_name = "Pi slice #";
        _slice_name += std::to_string(i);
        p_slice_pi->setName(_slice_name);
        pi_slice_pool.pushTask(static_cast<void *>(p_slice_pi));
        //task_vec.push_back(static_cast<BGeneralTask *>(p_slice_pi));
    }
    
    //pi_slice_pool.optimizer(task_vec, BGeneralThreadPool::Optimizer::PerformanceFirst);
    cout << "Current threads: " << pi_slice_pool.size() <<endl;
    pi_slice_pool.startAllTasks();
    
    //2. Calculate pi
    double Pi = 0.0;
    
    while(g_pi_slice_result.size() != slice_num)
    {
        std::cout << "Current result size: " << g_pi_slice_result.size() << std::endl;
        sleep(1);
    }
    
    for(int i=0;i < slice_num;i++)
    {
        Pi += g_pi_slice_result[i];
        std::cout << "Pi slice result is : " << g_pi_slice_result[i] << std::endl;
    }
    
    std::cout.setf(ios::showpoint);
    std::cout.precision(64);
    
    std::cout << "Interger Pi is : " << Pi << std::endl;
    Pi /= multiple/4;
    std::cout << "Pi is : " << Pi << std::endl;
    
    getchar();
    
    pi_slice_pool.kill();
	
    return 0;
}
