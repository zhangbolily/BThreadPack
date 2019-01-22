/*
 * @Author      : Ball Chang
 * @File        : BTimer.h
 * @Date        : 2018-12-19
*/

#ifndef _BTIMER_H_
#define _BTIMER_H_

#include <atomic>
#include <chrono>

#include "BThreadPack.h"

namespace BThreadPack {

class BTimer{

public:
    BTimer();
    ~BTimer();
    
    /* @start record time in milliseconds */
    void start();
    
    /* @stop record time */
    void stop();
    
    /* @time - Get how many milliseconds have beed recorded */
    long long time();
    
    void reset();
    
private:
    std::chrono::steady_clock::time_point m_start_time_us;
    std::chrono::steady_clock::time_point m_stop_time_us;
};
};

#endif
