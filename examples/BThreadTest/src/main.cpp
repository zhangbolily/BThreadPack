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

#include "BThreadPack/BThread.h"

using std::cout;
using BThreadPack::BThread;
using BThreadPack::BThreadInfo;

void run(BThreadInfo& thread_info) {
    thread_info.running();

    std::cout << "Worker thread: I'm running......" << std::endl;
    sleep(2);
    std::cout << "Walkout!!!" << std::endl;

    thread_info.exit(0);
}

int main(int argc, char** argv) {
    BThreadInfo thread_info;
    BThread infinite_run;
    infinite_run.start(run, thread_info);
    infinite_run.detach();

    while(1) {
        std::cout << "Main thread: I'm running......" << std::endl;
        if (infinite_run.isRunning())
            std::cout << "Main thread: Worker thread is running, very good." << std::endl;
        else {
            std::cout << "Main thread: Worker thread isn't running, very bad." << std::endl;
            return 0;
        }
        sleep(1);
    }
}
