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
 * @File        : BTimer.h
 * @Date        : 2018-12-19
*/

#ifndef INCLUDE_BUTILS_BTIMER_H_
#define INCLUDE_BUTILS_BTIMER_H_

#ifdef UNIX
#include <signal.h>
#endif

#include <atomic>
#include <chrono>
#include <ctime>
#include <functional>
#include <cstring>
#include <sstream>
#include <thread>
#include <utility>

#include "BCore/BCore.h"
#include "BCore/BDebug.h"
#include "BCore/BType.h"

#define MILLIAN 1000000

namespace BThreadPack {

using namespace BCore;
using std::chrono::steady_clock;
using std::chrono::duration_cast;
using std::chrono::microseconds;

class BTimer{
    enum BTimerStatus {
        Timing,
        Alarm,
        Stop
    };

 public:
    BTimer();
    ~BTimer();

    /* @start record time in milliseconds */
    void start();

    /* @start timer with specified duration */
    int32 start(int64 _msec);
    int32 start(std::chrono::milliseconds _msec);

    /* @stop record time or this timer */
    int32 stop();

    /* @time - Get how many milliseconds have beed recorded or remained */
    int64 time() const;

#ifndef WIN32
    void setInterval(int64 _msec);
    void setInterval(std::chrono::milliseconds _msec);

    template<class Function>
    void callOnTimeout(Function&& _f, void* _arg) {
        m_timeout_call_flag = true;
        m_timer_event.sigev_value.sival_ptr = _arg;
        m_timer_event.sigev_notify = SIGEV_THREAD;
        m_timer_event.sigev_notify_function = std::forward<Function>(_f);
    }

    template<class Function>
    void callOnTimeout(Function&& _f, int32 _arg) {
        m_timeout_call_flag = true;
        m_timer_event.sigev_value.sival_int = _arg;
        m_timer_event.sigev_notify = SIGEV_THREAD;
        m_timer_event.sigev_notify_function = std::forward<Function>(_f);
    }

    template<class Function>
    int32 callOnTimeout(Function&& _f, void* _arg, std::thread::id _pid) {
        std::ostringstream _pid_stream;
        _pid_stream << _pid;
        return callOnTimeout(std::forward<Function>(_f),
                _arg, static_cast<pid_t>(std::stoll(_pid_stream.str())));
    }

    template<class Function>
    int32 callOnTimeout(Function&& _f, void* _arg, pid_t _pid) {
        m_timeout_call_flag = true;
        m_timer_event.sigev_value.sival_ptr = _arg;
        m_timer_event.sigev_signo = SIGRTMIN;
        m_timer_event.sigev_notify = SIGEV_THREAD_ID;
        m_timer_event._sigev_un._tid = _pid;
        m_timer_action.sa_sigaction = std::forward<Function>(_f);
        m_timer_action.sa_flags = SA_SIGINFO;

        if (sigaction(SIGRTMIN, &m_timer_action, nullptr) == -1) {
            return BCore::ReturnCode::BError;
        } else {
            return BCore::ReturnCode::BSuccess;
        }
    }

    template<class Function>
    int32 callOnTimeout(Function&& _f, int32 _arg, std::thread::id _pid) {
        std::ostringstream _pid_stream;
        _pid_stream << _pid;
        return callOnTimeout(std::forward<Function>(_f),
                _arg, static_cast<pid_t>(std::stoll(_pid_stream.str())));
    }

    template<class Function>
    int32 callOnTimeout(Function&& _f, int32 _arg, pid_t _pid) {
        m_timeout_call_flag = true;
        m_timer_event.sigev_value.sival_int = _arg;
        m_timer_event.sigev_signo = SIGRTMIN;
        m_timer_event.sigev_notify = SIGEV_THREAD_ID;
        m_timer_event._sigev_un._tid = _pid;
        m_timer_action.sa_sigaction = std::forward<Function>(_f);
        m_timer_action.sa_flags = SA_SIGINFO;

        if (sigaction(SIGRTMIN, &m_timer_action, nullptr) == -1) {
            return BCore::ReturnCode::BError;
        } else {
            return BCore::ReturnCode::BSuccess;
        }
    }

    void reset();
#endif

 private:
    std::chrono::steady_clock::time_point m_start_time_us;
    std::chrono::steady_clock::time_point m_stop_time_us;
    BTimerStatus m_timer_status;

#ifndef WIN32
    sigevent m_timer_event;
    struct sigaction m_timer_action;
    timer_t m_timer_id;
    itimerspec m_timer_initial_value;
    bool m_timeout_call_flag;
#endif
};
}  // namespace BThreadPack

#endif  // INCLUDE_BUTILS_BTIMER_H_
