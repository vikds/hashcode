#pragma once

#include "fwd.hpp"

namespace hashcode
{

struct GreenLight {
    Street* street;
    size_t duration;
};

class TrafficLight {
public:
    TrafficLight(const std::vector<GreenLight>& schedule)
      : schedule_(schedule),
        direction_(0),
        tick_(0)
    {}

    void Tick();

private:
    void Switch();

private:
    std::vector<GreenLight> schedule_;
    size_t direction_;
    size_t tick_;
};

} // namespace hashcode