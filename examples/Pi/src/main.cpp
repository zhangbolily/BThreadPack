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
    
    virtual int process()
    {
    	ofstream out_file;
    	string fname = "./data/pi_result_" + to_string(m_task_id_) + ".b";
    	out_file.open(fname);
    	out_file<<"Different precision pi:\n";
    	for(int i=(PRECISION-100);i < PRECISION;i++)
    	{
    		out_file<<to_string(pi(i))<<"\n";
    	}
        out_file.close();
        cout<<"Pi is ready."<<endl;
    }

private:
    int m_task_id_;
    double pi(int _level)
    {
        bool sign = true;
        double _pi = 0.0;
        for(int i=0;i < _level;i++)
        {
            if(sign)
                _pi += 4000000000000.0/(i*2 + 1);
            else
                _pi -= 4000000000000.0/(i*2 + 1);
            sign = !sign;
        }
        return _pi;
    }
};

int num_threads = 20;
BGeneralThreadPool hello_world_pool(num_threads, BAbstractThreadPool::BThreadControlMode::DynamicThreadCapacity);

int main(int argc, char** argv)
{   
    vector<BGeneralTask *> task_vec;
    int task_num = 20;
    //1. Add task into thread pool.
    for(int i=0;i < task_num;i++)
    {
        CalculatePiTask* p_write_disk = new CalculatePiTask(i);
        //hello_world_pool.pushTask(static_cast<BGeneralTask *>(p_write_disk));
        task_vec.push_back(static_cast<BGeneralTask *>(p_write_disk));
    }
    
    hello_world_pool.optimizer(task_vec, BGeneralThreadPool::Optimizer::PerformanceFirst);
    cout << "Current threads: " << hello_world_pool.size() <<endl;
    //hello_world_pool.startAllTasks();
    
    getchar();
    
    hello_world_pool.kill();
    
    for(int i=0;i < task_num;i++)
    {
        delete static_cast<CalculatePiTask*>(task_vec[i]);
    }
	
    return 0;
}
