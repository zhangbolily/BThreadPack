/*
 * @Author	: Ball Chang
 * @File	: BThread.h
 * @Date	: 2018-11-11
*/

#ifndef _BTHREAD_H_
#define _BTHREAD_H_

#include <pthread.h>

namespace BThreadPack {

class BThread{

public:
	BThread(int _detach);
	~BThread();

	/* @kill - Kill this thread and release all resources.
	 * Don't need any parameters.
	*/
	void kill();

private:
	int _pid_;
	int _detach_;

};

};

#endif
