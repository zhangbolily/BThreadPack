/*
 * @Author      : Ball Chang
 * @File        : BTimer.cpp
 * @Date        : 2018-12-19
*/

#include "BUtils/BTimer.h"

using namespace std;
using namespace chrono;

namespace BThreadPack {

BTimer::BTimer()
    :m_timer_id(nullptr),
    m_timeout_call_flag(false)
{
    memset(&m_timer_event, 0, sizeof(sigevent));
    memset(&m_timer_action, 0, sizeof(struct sigaction));
    memset(&m_timer_initial_value, 0, sizeof(itimerspec));
}

BTimer::~BTimer()
{
}

void BTimer::start()
{
    m_timer_mode = BTimer::TimingMode;
    m_start_time_us = steady_clock::now();
}

int BTimer::start(long long _msec)
{
    m_timer_mode = BTimer::AlarmMode;
    m_start_time_us = steady_clock::now();
    m_timer_initial_value.it_value.tv_sec = _msec / 1000;
    m_timer_initial_value.it_value.tv_nsec = static_cast<long>(_msec % 1000 * MILLIAN);
#ifdef _B_DEBUG_
    B_PRINT_DEBUG("BTimer::start - m_timer_initial_value.it_value.tv_sec is "<<m_timer_initial_value.it_value.tv_sec<<".")
    B_PRINT_DEBUG("BTimer::start - m_timer_initial_value.it_value.tv_nsec is "<<m_timer_initial_value.it_value.tv_nsec<<".")
#endif
    // There is an existing timer, delete it first.
    if (m_timer_id != nullptr) {
        timer_delete(m_timer_id);
        m_timer_id = nullptr;
    }
    
    if (timer_create(CLOCK_REALTIME, &m_timer_event, &m_timer_id) == -1)
	{
#ifdef _B_DEBUG_
    B_PRINT_DEBUG("BTimer::start - m_timer_id is "<<m_timer_id<<".")
#endif
        return BCore::ReturnCode::BError;
	} else if (timer_settime(m_timer_id, 0, &m_timer_initial_value, nullptr) == -1){
#ifdef _B_DEBUG_
    B_PRINT_DEBUG("BTimer::start - m_timer_id is "<<m_timer_id<<".")
#endif
	    return BCore::ReturnCode::BError;
	} else {
#ifdef _B_DEBUG_
    B_PRINT_DEBUG("BTimer::start - m_timer_id is "<<m_timer_id<<".")
#endif
	    return BCore::ReturnCode::BSuccess;
	}
}

int BTimer::start(std::chrono::milliseconds _msec)
{
    return start(_msec.count());
}

int BTimer::stop()
{
    m_stop_time_us = steady_clock::now();
    switch(m_timer_mode){
        case BTimerMode::TimingMode: {
        };
        case BTimerMode::AlarmMode:{
            if (m_timer_id != nullptr) {
                if(timer_delete(m_timer_id) == -1)
                    return BCore::ReturnCode::BError;
                else {
                    m_timer_id = nullptr;
                    return BCore::ReturnCode::BSuccess;
                }
            }
        };
        default:return BCore::ReturnCode::BSuccess;
    }
}

void BTimer::setInterval(long long _msec)
{
    m_timer_initial_value.it_interval.tv_sec = _msec / 1000;
    m_timer_initial_value.it_interval.tv_nsec = static_cast<long>(_msec % 1000 * MILLIAN);
#ifdef _B_DEBUG_
    B_PRINT_DEBUG("BTimer::setInterval - m_timer_initial_value.it_interval.tv_sec is "<<m_timer_initial_value.it_interval.tv_sec<<".")
    B_PRINT_DEBUG("BTimer::setInterval - m_timer_initial_value.it_interval.tv_nsec is "<<m_timer_initial_value.it_interval.tv_nsec<<".")
#endif
}

void BTimer::setInterval(std::chrono::milliseconds _msec)
{
    setInterval(_msec.count());
}

long long BTimer::time() const
{
    if (m_stop_time_us < m_start_time_us)
    {
        return BCore::ReturnCode::BError;
    }else
        return duration_cast<microseconds>(m_stop_time_us - m_start_time_us).count();
}

void BTimer::reset()
{
}

}
