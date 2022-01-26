#pragma once

#include "fwd.hpp"

#include <chrono>

namespace hashcode
{

 using Time = std::chrono::high_resolution_clock::time_point;

class Timer {
public:
    Timer(const std::string& message);
    ~Timer();

private:
    const std::string message_;
    Time start_;
};

} // namespace hashcode