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

using namespace std;
using namespace BThreadPack;

class WriteDiskTask: public BGeneralTask{
public:
    WriteDiskTask(int _task_id)
        :BGeneralTask(),
        m_task_id_(_task_id)
    {
    }
    
    ~WriteDiskTask()
    {
    }
    
    virtual int process()
    {
    	ofstream out_file;
    	string fname = "./data/sin_result_task_" + to_string(m_task_id_) + ".btk";
    	out_file.open(fname);
    	out_file<<"This file include many result of pow(tan(i), cos(i))/pow(sin(i), cos(i)).\n";
    	for(int i=0;i < 40000;i++)
    	{
    		out_file<<to_string(pow(tan(i), cos(i))/pow(sin(i), cos(i)))<<"\n";
    	}
        out_file.close();
    }

private:
    int m_task_id_;
};

int num_threads = 20;
BGeneralThreadPool hello_world_pool(num_threads, BAbstractThreadPool::BThreadControlMode::DynamicThreadCapacity);

int main(int argc, char** argv)
{   
    vector<BGeneralTask *> task_vec;
    //1. Add task into thread pool.
    for(int i=0;i < 100;i++)
    {
        WriteDiskTask* p_write_disk = new WriteDiskTask(i);
        task_vec.push_back(static_cast<BGeneralTask *>(p_write_disk));
    }
    
    hello_world_pool.optimizer(task_vec, BGeneralThreadPool::Optimizer::PerformanceFirst);
    cout << "Current threads: " << hello_world_pool.size() <<endl;
    
    getchar();
    
    hello_world_pool.kill();
    
    for(int i=0;i < 100;i++)
    {
        delete static_cast<WriteDiskTask*>(task_vec[i]);
    }
	
    return 0;
}
