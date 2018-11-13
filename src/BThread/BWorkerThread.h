/*
 * @Author      : Ball Chang
 * @File        : BWorkerThread.h
 * @Date        : 2018-11-11
*/

#ifndef _BWORKER_THREAD_H_
#define _BWORKER_THREAD_H_

#include <pthread.h>
#include <atomic>
#include "BAbstractThread.h"

using namespace std;

namespace BThreadPack {

class BWorkerThread : public BAbstractThread{

public:
	BWorkerThread(bool _detach);
	~BWorkerThread();

	/* @kill - Kill this thread and release all resources.
	 * Don't need any parameters.
	*/
	void kill();

private:

};

};

#endif
