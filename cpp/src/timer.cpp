#include "timer.hpp"

namespace hashcode
{

Timer::Timer(const std::string& message)
  : message_(message)
{
    start_ = std::chrono::high_resolution_clock::now();
}

Timer::~Timer() {
    Time end = std::chrono::high_resolution_clock::now();
    size_t spend_sec = std::chrono::duration_cast<std::chrono::seconds>(end - start_).count();
    size_t spend_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start_).count();
    std::cout << message_ << " (" << spend_ms << " ms)" << std::endl;
}

} // namespace hashcode