/*
 * @Author	: Ball Chang
 * @File	: BThread.h
 * @Date	: 2018-11-11
*/

#include <pthread.h>

namespace BThreadPack {

class BThread{

public:
	BThread(int _detach);
	~BThread();

	/* @kill - Kill this thread and release all resources.
	 * Don't need any parameters.
	*/
	kill();

private:
	int _pid_;

};

};
