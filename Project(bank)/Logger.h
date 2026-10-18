#pragma once

#include <iostream>
#include <mutex>

namespace Logger {
    inline std::mutex mtx;

    template<typename T,typename... Args>
    void Log(T first,Args... args) {
        std::lock_guard<std::mutex> lock(mtx);
        std::cout << first;
        (std::cout << ... << args);
        std::cout << '\n';
    }
};