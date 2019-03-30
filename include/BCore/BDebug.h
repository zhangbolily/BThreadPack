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
 * @File        : BDebug.h
 * @Date        : 2019-3-20
*/

#ifndef INCLUDE_BCORE_BDEBUG_H_
#define INCLUDE_BCORE_BDEBUG_H_

#define _B_DEBUG_

#ifdef _B_DEBUG_
#include <sstream>
#include <iostream>
#include <ctime>
#include <thread>
#endif

/*
 *  Definition of return code
*/
#ifdef _B_DEBUG_
/*
 *  Definition of print debug info
 *  Use ostringstream to make sure the debug message can be output in synchronized format
*/

#define B_PRINT_DEBUG(msg)  \
{\
    std::ostringstream _os;\
    std::time_t result = std::time(nullptr);\
    char time_str[64] = {0};\
    std::strftime(time_str, sizeof(time_str), \
    "%Y-%m-%d %H:%M:%S ", std::localtime(&result));\
    _os << "[Debug] Thread "<< std::this_thread::get_id()\
    << " " << time_str << msg << "\n";\
    std::cout << _os.str();\
}

/*
 *  Definition of print debug info
*/

#define B_END_OF_DEBUG(msg)  \
{\
    std::ostringstream _os;\
    _os << "[Debug] " << msg << "\n";\
    std::cout << _os.str();\
}

/*
 *  Definition of print error info
*/

#define B_PRINT_ERROR(msg)  \
{\
    std::ostringstream _os;\
    std::time_t result = std::time(nullptr);\
    char time_str[64] = {0};\
    std::strftime(time_str, sizeof(time_str), \
    "%Y-%m-%d %H:%M:%S ", std::localtime(&result));\
    _os << "\033[37;41m[Error] Thread "<< std::this_thread::get_id() \
    << " " << time_str << msg << "\033[0m\n";\
    std::cout << _os.str();\
}
#endif

#endif  // INCLUDE_BCORE_BDEBUG_H_
