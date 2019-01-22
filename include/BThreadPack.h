/*
 * @Author      : Ball Chang
 * @File        : BThreadPack.h
 * @Date        : 2018-11-17
*/
#ifdef _B_DEBUG_
#include <sstream>
#include <iostream>
#include <ctime>
#include <thread>
#endif

#ifndef _BTHREAD_PACK_H_
#define _BTHREAD_PACK_H_

/*
 *  Definition of return code
*/

namespace BThreadPack{
    enum ReturnCode{
        BError = -1,
        BSuccess = 0,
        BOnlySingleThread = -2, //Accessed by more than one thread
        BThreadPoolFull = -3,   //Threads more than the thread pool capacity
        BThreadNotExists = -4,  //Specified thread number doesn't exist
        BModeIncorrect = -5     //Required running mode is not satisfied
    };
};

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
    std::strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S ", std::localtime(&result));\
    _os << "[Debug] Thread "<< this_thread::get_id() << " " << time_str << msg << "\n";\
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
    std::strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S ", std::localtime(&result));\
    _os << "\033[37;41m[Error] Thread "<< this_thread::get_id() << " " << time_str << msg << "\033[0m\n";\
    std::cout << _os.str();\
}
#endif

#endif
