/* MIT License
* 
* Copyright (c) 2018 Ball Chang
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

/*
 * @Author      : Ball Chang
 * @File        : BTimer.cpp
 * @Date        : 2018-12-19
*/

#include "BUtils/BTimer.h"

namespace BUtils {

BTimer::BTimer()
#ifdef WIN32
    :m_timer_status(BTimer::BTimerStatus::Stop) {
#else
    :m_timer_status(BTimer::BTimerStatus::Stop),
     m_timer_id(nullptr),
     m_timeout_call_flag(false) {
#endif
#ifndef WIN32
    memset(&m_timer_event, 0, sizeof(sigevent));
    memset(&m_timer_action, 0, sizeof(struct sigaction));
    memset(&m_timer_initial_value, 0, sizeof(itimerspec));
#endif
}

BTimer::~BTimer() {
}

void BTimer::start() {
    if (m_timer_status == BTimer::Stop) {
        m_timer_status = BTimer::Timing;
        m_start_time_us = steady_clock::now();
    }
}

int32 BTimer::start(int64 _msec) {
    if (m_timer_status == BTimer::Stop) {
        m_timer_status = BTimer::Alarm;
        m_start_time_us = steady_clock::now();

#ifndef WIN32
        m_timer_initial_value.it_value.tv_sec = _msec / 1000;
        m_timer_initial_value.it_value.tv_nsec =
                static_cast<int64>(_msec % 1000 * MILLIAN);
#ifdef _B_DEBUG_
        B_PRINT_DEBUG("BTimer::start -"
                      " m_timer_initial_value.it_value.tv_sec is "
        << m_timer_initial_value.it_value.tv_sec << ".")
        B_PRINT_DEBUG("BTimer::start -"
                      " m_timer_initial_value.it_value.tv_nsec is "
        << m_timer_initial_value.it_value.tv_nsec << ".")
#endif
        // There is an existing timer, delete it first.
        if (m_timer_id != nullptr) {
            timer_delete(m_timer_id);
            m_timer_id = nullptr;
        }

        if (timer_create(CLOCK_REALTIME, &m_timer_event, &m_timer_id) == -1) {
#ifdef _B_DEBUG_
            B_PRINT_DEBUG("BTimer::start - m_timer_id is " << m_timer_id << ".")
#endif
            return BCore::ReturnCode::BError;
        } else if (timer_settime(m_timer_id, 0, &m_timer_initial_value, nullptr)
                    == -1) {
#ifdef _B_DEBUG_
            B_PRINT_DEBUG("BTimer::start - m_timer_id is " << m_timer_id << ".")
#endif
            return BCore::ReturnCode::BError;
        } else {
#ifdef _B_DEBUG_
            B_PRINT_DEBUG("BTimer::start - m_timer_id is " << m_timer_id << ".")
#endif
            return BCore::ReturnCode::BSuccess;
        }
#endif
    }

    return BCore::ReturnCode::BSuccess;
}

int32 BTimer::start(std::chrono::milliseconds _msec) {
    return start(_msec.count());
}

int32 BTimer::stop() {
    m_stop_time_us = steady_clock::now();
#ifndef WIN32
    switch (m_timer_status) {
        case BTimerStatus::Timing: {
            m_timer_status = BTimer::Stop;
        };
        case BTimerStatus::Alarm: {
            if (m_timer_id != nullptr) {
                if (timer_delete(m_timer_id) == -1) {
                    m_timer_status = BTimer::Stop;
                    return BCore::ReturnCode::BError;
                } else {
                    m_timer_id = nullptr;
                    m_timer_status = BTimer::Stop;
                    return BCore::ReturnCode::BSuccess;
                }
            }
        };
        default:return BCore::ReturnCode::BSuccess;
    }
#endif
    return BCore::ReturnCode::BSuccess;
}

int64 BTimer::time() const {
    if (m_stop_time_us < m_start_time_us) {
        return BCore::ReturnCode::BError;
    } else {
        return duration_cast<microseconds>(
                m_stop_time_us - m_start_time_us).count();
    }
}


#ifndef WIN32
void BTimer::setInterval(int64 _msec) {
    m_timer_initial_value.it_interval.tv_sec = _msec / 1000;
    m_timer_initial_value.it_interval.tv_nsec =
            static_cast<int64>(_msec % 1000 * MILLIAN);
#ifdef _B_DEBUG_
    B_PRINT_DEBUG("BTimer::setInterval -"
                  " m_timer_initial_value.it_interval.tv_sec is "
                  << m_timer_initial_value.it_interval.tv_sec << ".")
    B_PRINT_DEBUG("BTimer::setInterval -"
                  " m_timer_initial_value.it_interval.tv_nsec is "
                  << m_timer_initial_value.it_interval.tv_nsec << ".")
#endif
}

void BTimer::setInterval(std::chrono::milliseconds _msec) {
    setInterval(_msec.count());
}

void BTimer::reset() {
}
#endif
}  // namespace BThreadPack
