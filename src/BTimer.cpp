/*
 * @Author      : Ball Chang
 * @File        : BTimer.cpp
 * @Date        : 2018-12-19
*/

#include "BTimer.h"

using namespace std;
using namespace chrono;

namespace BThreadPack {

BTimer::BTimer()
{
}

BTimer::~BTimer()
{
}

void BTimer::start()
{
    m_start_time_us = steady_clock::now();
}

void BTimer::stop()
{
    m_stop_time_us = steady_clock::now();
}

long long BTimer::time()
{
    if (m_stop_time_us < m_start_time_us)
    {
        return B_ERROR;
    }else
        return duration_cast<microseconds>(m_stop_time_us - m_start_time_us).count();
}

void BTimer::reset()
{
}

}
