#pragma once

#include "fwd.hpp"

namespace hashcode
{

struct StreetLight {
    StreetLight(Street* street, size_t duration);

    Street* street;
    size_t duration;
};

class TrafficLight {
public:
    TrafficLight(Intersection* intersection, const Schedule& schedule);

    Intersection& intersection() {
        return *intersection_;
    }
    Schedule& schedule() {
        return schedule_;
    }

public:
    void Tick(size_t time);

    void Reset();

private:
    void Switch();

private:
    Intersection* intersection_;
    Schedule schedule_;
    size_t direction_;
    size_t tick_;
};

} // namespace hashcode