#include "Timer.h"
#include <thread>
#include <string>

void Timer::once(int delay,
                   std::function<void()> callback,
                   bool async) {
    if (async) {
        std::thread([=]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(delay));
            callback();
        }).detach();
    }
    else {
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        callback();
    }
}

template<typename T>
void Timer::once(int delay,
                   std::function<void(T)> callback,
                   T arg,
                   bool async) {
    if (async) {
        std::thread([=]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(delay));
            callback(arg);
        }).detach();
    }
    else {
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        callback(arg);
    }
}

void Timer::repeted(int delay, int count, std::function<void()> callback, bool async) {
    if (async) {
        std::thread([=]() {
            for (int i = 0; i < count; i++) {
                std::this_thread::sleep_for(std::chrono::milliseconds(delay));
                callback();
            }
        }).detach();
    }
    else {
        for (int i = 0; i < count; i++) {
            std::this_thread::sleep_for(std::chrono::milliseconds(delay));
            callback();
        }
    }
}

template<typename T>
void Timer::repeted(int delay, int count, std::function<void(T)> callback, T arg, bool async) {
    if (async) {
        std::thread([=]() {
            for (int i = 0; i < count; i++) {
                std::this_thread::sleep_for(std::chrono::milliseconds(delay));
                callback(arg);
            }
        }).detach();
    }
    else {
        for (int i = 0; i < count; i++) {
            std::this_thread::sleep_for(std::chrono::milliseconds(delay));
            callback(arg);
        }
    }
}

void Timer::nonStop(int delay, std::function<void()> callback, bool async) {
    if (async) {
        std::thread([=]() {
            while (true) {
                std::this_thread::sleep_for(std::chrono::milliseconds(delay));
                callback();
            }
        }).detach();
    }
    else {
        while (true) {
            std::this_thread::sleep_for(std::chrono::milliseconds(delay));
            callback();
        }
    }
}

template<typename T>
void Timer::nonStop(int delay, std::function<void(T)> callback, T arg, bool async) {
    if (async) {
        std::thread([=]() {
            while (true) {
                std::this_thread::sleep_for(std::chrono::milliseconds(delay));
                callback(arg);
            }
        }).detach();
    }
    else {
        while (true) {
            std::this_thread::sleep_for(std::chrono::milliseconds(delay));
            callback(arg);
        }
    }
}

template void Timer::once<std::string*>(int, std::function<void(std::string*)>, std::string*, bool);
template void Timer::once<int*>(int, std::function<void(int*)>, int*, bool);

template void Timer::repeted<std::string*>(int, int, std::function<void(std::string*)>, std::string*, bool);
template void Timer::repeted<int*>(int, int, std::function<void(int*)>, int*, bool);

template void Timer::nonStop<std::string*>(int, std::function<void(std::string*)>, std::string*, bool);
template void Timer::nonStop<int*>(int, std::function<void(int*)>, int*, bool);