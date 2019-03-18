/*
 * @Author      : Ball Chang
 * @File        : BGeneralTask.cpp
 * @Date        : 2018-12-3
*/

#include "BThreadPack.h"
#include "BGeneralTask.h"

namespace BThreadPack{
   
BGeneralTask::BGeneralTask()
    :BAbstractTask()
{
}

BGeneralTask::BGeneralTask(bool _autodestroy)
    :BAbstractTask(_autodestroy)
{
}
   
BGeneralTask::BGeneralTask(const void* _buffer, size_t _size)
    :BAbstractTask(_buffer, _size)
{
}

BGeneralTask::BGeneralTask(bool _autodestroy, const void* _buffer, size_t _size)
    :BAbstractTask(_autodestroy, _buffer, _size)
{
}

void BGeneralTask::startExecutionTiming()
{
    m_execute_timer.start();
}

void BGeneralTask::stopExecutionTiming()
{
    m_execute_timer.stop();
}

long long BGeneralTask::executionTime()
{
    return m_execute_timer.time();
}

void BGeneralTask::startRealTiming()
{
    m_real_timer.start();
}

void BGeneralTask::stopRealTiming()
{
     m_real_timer.stop();
}

long long BGeneralTask::realTime()
{
    return m_real_timer.time();
}

BGeneralTask::~BGeneralTask()
{
}

void BGeneralTask::setName(const char* _name)
{
    m_name = _name;
}

void BGeneralTask::setName(std::string _name)
{
    m_name = _name;
}

const std::string& BGeneralTask::name() const
{
    return m_name;
}

};
