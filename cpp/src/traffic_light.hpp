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

    Intersection& intersection() const {
        return *intersection_;
    }
    Schedule& schedule() {
        return schedule_;
    }
    size_t scheduled_streets() const;

public:
    void Reset();
    void Tick();

private:
    StreetLight& GetNextScheduledStreet();
    void Switch(StreetLight& curr_street_light);

private:
    Intersection* intersection_;
    Schedule schedule_;
    size_t direction_;
    size_t scheduled_streets_;
    size_t tick_;
};

} // namespace hashcode