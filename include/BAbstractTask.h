/*
 * @Author      : Ball Chang
 * @File        : BAbstractTask.h
 * @Date        : 2018-11-12
*/

#ifndef _BABSTRACT_TASK_H_
#define _BABSTRACT_TASK_H_

#include <atomic>

using namespace std;

namespace BThreadPack {

class BAbstractTask{

public:
	enum class BTaskStatus{
		TaskFailed = -1,
		TaskInit = 0,
		TaskDoing = 1,
		TaskFinished = 2
	};

	BAbstractTask();
	~BAbstractTask();

private:
	atomic_int _taskStatus_;

};

};

#endif
