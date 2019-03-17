/*
 * @Author      : Ball Chang
 * @File        : BTimer.h
 * @Date        : 2018-12-19
*/

#ifndef _BTIMER_H_
#define _BTIMER_H_

#include <atomic>
#include <chrono>
#include <ctime>
#include <functional>
#include <signal.h>
#include <string.h>
#include <sstream>
#include <thread>

#include "BThreadPack/BThreadPack.h"

#define MILLIAN 1000000

namespace BThreadPack {

class BTimer{
    enum BTimerMode {
        TimingMode,
        AlarmMode
    };

public:
    BTimer();
    ~BTimer();
    
    /* @start record time in milliseconds */
    void start();
    
    /* @start timer with specified duration */
    int start(long long _msec);
    int start(std::chrono::milliseconds _msec);
    
    /* @stop record time or this timer */
    int stop();
    
    void setInterval(long long _msec);
    void setInterval(std::chrono::milliseconds _msec);
    
    /* @time - Get how many milliseconds have beed recorded or remained */
    long long time() const;
    
    template<class Function>
    void callOnTimeout(Function&& _f, void* _arg)
    {
        m_timeout_call_flag = true;
        m_timer_event.sigev_value.sival_ptr = _arg;
        m_timer_event.sigev_notify = SIGEV_THREAD;
        m_timer_event.sigev_notify_function = std::forward<Function>(_f);
    }
    
    template<class Function>
    void callOnTimeout(Function&& _f, int _arg)
    {
        m_timeout_call_flag = true;
        m_timer_event.sigev_value.sival_int = _arg;
        m_timer_event.sigev_notify = SIGEV_THREAD;
        m_timer_event.sigev_notify_function = std::forward<Function>(_f);
    }
    
    template<class Function>
    int callOnTimeout(Function&& _f, void* _arg, std::thread::id _pid)
    {
        std::ostringstream _pid_stream;
        _pid_stream << _pid;
        return callOnTimeout(std::forward<Function>(_f), _arg, static_cast<pid_t>(std::stoll(_pid_stream.str())));
    }
    
    template<class Function>
    int callOnTimeout(Function&& _f, void* _arg, pid_t _pid)
    {
        m_timeout_call_flag = true;
        m_timer_event.sigev_value.sival_ptr = _arg;
        m_timer_event.sigev_signo = SIGRTMIN;
        m_timer_event.sigev_notify = SIGEV_THREAD_ID;
        m_timer_event._sigev_un._tid = _pid;
        m_timer_action.sa_sigaction = std::forward<Function>(_f);
        m_timer_action.sa_flags = SA_SIGINFO;
        
        if (sigaction(SIGRTMIN, &m_timer_action, nullptr) == -1)
        {
            return ReturnCode::BError;
        } else {
            return ReturnCode::BSuccess;
        }
    }
    
    template<class Function>
    int callOnTimeout(Function&& _f, int _arg, std::thread::id _pid)
    {
        std::ostringstream _pid_stream;
        _pid_stream << _pid;
        return callOnTimeout(std::forward<Function>(_f), _arg, static_cast<pid_t>(std::stoll(_pid_stream.str())));
    }
    
    template<class Function>
    int callOnTimeout(Function&& _f, int _arg, pid_t _pid)
    {
        m_timeout_call_flag = true;
        m_timer_event.sigev_value.sival_int = _arg;
        m_timer_event.sigev_signo = SIGRTMIN;
        m_timer_event.sigev_notify = SIGEV_THREAD_ID;
        m_timer_event._sigev_un._tid = _pid;
        m_timer_action.sa_sigaction = std::forward<Function>(_f);
        m_timer_action.sa_flags = SA_SIGINFO;
        
        if (sigaction(SIGRTMIN, &m_timer_action, nullptr) == -1)
        {
            return ReturnCode::BError;
        } else {
            return ReturnCode::BSuccess;
        }
    }
    
    void reset();
    
private:
    std::chrono::steady_clock::time_point m_start_time_us;
    std::chrono::steady_clock::time_point m_stop_time_us;
    sigevent m_timer_event;
    struct sigaction m_timer_action;
    timer_t m_timer_id;
    itimerspec m_timer_initial_value;
    bool m_timeout_call_flag;
    BTimerMode m_timer_mode;
};
};

#endif
