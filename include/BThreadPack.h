/*
 * @Author      : Ball Chang
 * @File        : BThreadPack.h
 * @Date        : 2018-11-17
*/
#ifdef _B_DEBUG_
#include <sstream>
#include <ctime>
#include <thread>
#endif

#include "BAbstractTask.h"
#include "BAbstractThread.h"
#include "BWorkerThread.h"

#ifndef _BTHREAD_PACK_H_
#define _BTHREAD_PACK_H_

/*
 *  Definition of return code
*/

/* If an object can only run in a single thread was used in
 * multi-thread mode, this error code will be returned.
*/
#define B_ONLY_SINGLE_THREAD    -2

/*General error code.
*/
#define B_ERROR                 -1    

/*General success code.
*/                                   
#define B_SUCCESS               0

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
    _os << "[Error] " << msg << "\n";\
    std::cout << _os.str();\
}
#endif

#endif
