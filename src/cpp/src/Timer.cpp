#include "Timer.h"

Timer::Timer(std::string name) : name(name) {
    c_end = Clock::now();       // allows easy check if stop() was already called
    c_start = Clock::now();     // start at initialisation, may call start() again if needed
}

void Timer::start(){   // allow convenient rename on restart
    c_start = Clock::now();
}

void Timer::start(std::string name){   // allow convenient rename on restart
    this->name = name;
    start();
}

double Timer::getElapsed(){
    time_point temp =  c_end > c_start ? c_end : Clock::now();
    auto diff = temp - c_start; // durration object in ns
    return diff.count() / 1000000000.0;
}

std::string Timer::getName(){
    return name;
}

void Timer::stop(){
    c_end = Clock::now();
}

void Timer::print(){
    std::cout << name << "\t" << getElapsed() << "s" << std::endl;
}
