/*
 * @Author      : Ball Chang
 * @File        : BCore.h
 * @Date        : 2019-3-20
*/

#ifndef _B_CORE_H_
#define _B_CORE_H_

/*
 *  Definition of return code
*/

namespace BCore{
    enum ReturnCode{
        BError = -1,
        BSuccess = 0,
        BOnlySingleThread = -2, //Accessed by more than one thread
        BThreadPoolFull = -3,   //Threads more than the thread pool capacity
        BThreadNotExists = -4,  //Specified thread number doesn't exist
        BModeIncorrect = -5,     //Required running mode is not satisfied
        BParameterOutOfRange = -6
    };
};

#endif
