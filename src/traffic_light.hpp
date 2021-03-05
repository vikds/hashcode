#pragma once

#include "fwd.hpp"

namespace hashcode
{

struct StreetLight {
    StreetLight(Street* street, size_t duration);

    Street* street;
    size_t duration;
};

using Schedule = std::vector<StreetLight>; 

class TrafficLight {
public:
    TrafficLight(const Intersection& intersection, const Schedule& schedule);

    size_t scheduled_streets() const {
        return scheduled_streets_;
    }

public:
    void Reset();
    void Tick();

private:
    size_t CountScheduledStreets();
    StreetLight& GetNextScheduledStreet();
    void Switch(StreetLight& curr_street_light);

public:
    const Intersection* intersection;
    Schedule schedule;

private:
    size_t clock_ = 0;
    size_t direction_ = 0;
    size_t scheduled_streets_;
};

} // namespace hashcode