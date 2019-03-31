/* MIT License
*
* Copyright (c) 2018 Ball Chang
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

/*
 * @Author      : Ball Chang
 * @File        : main.cpp
 * @Date        : 2019-3-26
*/

#include <iostream>
#include <thread>
#include <unistd.h>
#include <ctime>

#include "BThreadPack/BThread.h"
#include "BUtils/BTimer.h"

using std::cout;
using BThreadPack::BThread;
using BThreadPack::BThreadInfo;
using BUtils::BTimer;

BTimer execution_timer;
BTimer real_timer;
std::clock_t cpu_time_start;
std::clock_t cpu_time_stop;

void run(BThreadInfo& thread_info) {
    cpu_time_stop = std::clock();
    execution_timer.start();
    thread_info.running();

    std::cout << "Worker thread: I'm running......" << std::endl;
    sleep(3);
    std::cout << "Worker thread: Walkout!!!" << std::endl;

    thread_info.exit(0);
    execution_timer.stop();
    real_timer.stop();
}

int main(int argc, char** argv) {
    cpu_time_start = std::clock();

    real_timer.start();
    BThreadInfo thread_info;
    BThread infinite_run;
    infinite_run.start(run, thread_info);
    infinite_run.detach();

    sleep(1);

    while(1) {
        std::cout << "Main thread: I'm running......" << std::endl;
        if (infinite_run.isRunning())
            std::cout << "Main thread: Worker thread #" << infinite_run.id() << " is running, very good." << std::endl;
        else {
            std::cout << "Main thread: Worker thread #" << infinite_run.id() << " isn't running, very bad." << std::endl;
            break;
        }
        sleep(1);
    }

    std::cout << "Create thread costs :" << 1000000*(cpu_time_stop - cpu_time_start)/CLOCKS_PER_SEC << " us" << std::endl;
    std::cout << "All cpu time is :" << 1000000*(std::clock() - cpu_time_start)/CLOCKS_PER_SEC << " us" << std::endl;
    std::cout << "Group task Pi :" << std::endl;
    std::cout << "Schedule time: " <<
            real_timer.time().count() - execution_timer.time().count() << " us" << std::endl;
    std::cout << "Real time: " <<
            real_timer.time().count() << " us" << std::endl;
    std::cout << "Execution time: " <<
            execution_timer.time().count() << " us" << std::endl;
}
