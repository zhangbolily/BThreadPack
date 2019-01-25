#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>

#include "BThreadPack/BThreadPack.h"
#include "BThreadPack/BTimer.h"

using namespace std;
using namespace BThreadPack;

void print(sigval _sig)
{
    cout<<"This is timer."<<endl;
    cout<<static_cast<char*>(_sig.sival_ptr)<<endl;
}

int main(int argc, char** argv)
{
    char line[] = "Only this is my target.";
	BTimer timer;
	cout<<std::this_thread::get_id()<<endl;
	timer.callOnTimeout(print, static_cast<void*>(line));
	timer.setInterval(0);
	if(timer.start(2000) != ReturnCode::BSuccess)
	    cout<<"Start timer failed!"<<endl;
	pause();
    return 0;
}
