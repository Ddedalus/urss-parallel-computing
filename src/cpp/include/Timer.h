#ifndef __TIMER_H__
#define __TIMER_H__

#include <chrono>
#include <iostream>
#include <string>

/**
 * @brief timer returning wall-clock time in seconds. System time is used.
 * Timer is auto-started on constructor call. May be stopped, reseted and renamed.
 * No pause functionality available.
 */
class Timer{
    using Clock = std::chrono::system_clock;
    using time_point = std::chrono::time_point<Clock>;
    time_point c_start;
    time_point c_end;
    std::string name;

    public:
    Timer(std::string name="");

    void start();

    /**
     * @brief Discard any previous result and start timing.
     * 
     * @param name new name for the timer.
     */
    void start(std::string name);
    /**
     * @brief Get time elapsed from the last timer
     * This doesn't stop the timer.
     * @return double in seconds
     */
    double getElapsed();

    std::string getName();

    void stop();

    /**
     * @brief Cout name with duration. Doesn't stop the timer.
     * 
     */
    void print();

};

#endif