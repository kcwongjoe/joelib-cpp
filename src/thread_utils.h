#pragma once
#ifndef JW_THREAD_UTILS_H
#define JW_THREAD_UTILS_H

//************Content************
#include <atomic>
#include <thread>
#include <functional>

namespace Utils
{

    // waitingForFinish
    bool waitingForFinish(std::atomic<bool>* stopWaiting, int delayms = 10, int timeout = 3000);
    bool waitingForFinish(std::function<void(std::atomic<bool>*)> func, int delayms = 10, int timeout = 3000);
    bool waitingForFinish(std::atomic<bool>* stopWaiting, std::function<void()> func, int delayms = 10, int timeout = 3000);
    bool waitingForFinish(std::atomic<bool>* stopWaiting, std::function<void(std::atomic<bool>*)> func, int delayms = 10, int timeout = 3000);

    
}


//*******************************

#endif