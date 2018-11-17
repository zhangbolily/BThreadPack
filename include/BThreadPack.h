/*
 * @Author      : Ball Chang
 * @File        : BThreadPack.h
 * @Date        : 2018-11-17
*/

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

#endif
