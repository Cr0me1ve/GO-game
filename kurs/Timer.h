#pragma once
#include <chrono>
#include <functional>

class Timer {
public:
    Timer() {};
    void once(int delay,        // call function once
             std::function<void()> callback,
             bool async = true);
    template <typename T>
    void once(int delay,        // call function once
             std::function<void(T)> callback,
             T arg,
             bool async = true);

    void repeted(int delay,     // call function repeatedly
                 int count,
                 std::function<void()> callback,
                 bool async = true);
    template <typename T>
    void repeted(int delay,     // call function repeatedly
                 int count,
                 std::function<void(T)> callback,
                 T arg,
                 bool async = true);
    
    void nonStop(int delay,     // call function endlessly
                 std::function<void()> callback,
                 bool async = true);
    template <typename T>
    void nonStop(int delay,     // call function endlessly
                 std::function<void(T)> callback,
                 T arg,
                 bool async = true);

};