/*
 * @Author      : Ball Chang
 * @File        : BDebug.h
 * @Date        : 2019-3-20
*/

#define _B_DEBUG_

#ifdef _B_DEBUG_
#include <sstream>
#include <iostream>
#include <ctime>
#include <thread>
#endif

#ifndef _B_DEBUG_H_
#define _B_DEBUG_H_

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
