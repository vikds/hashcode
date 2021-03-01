#pragma once

#include "fwd.hpp"

namespace hashcode
{

struct GreenLight {
    GreenLight(Street* street, size_t duration);

    Street* street;
    size_t duration;
};

class TrafficLight {
public:
    TrafficLight(size_t intersection_id, const Schedule& schedule);

    size_t intersection_id() const {
        return intersection_id_;
    }
    const Schedule& schedule() const {
        return schedule_;
    }

public:
    void Tick(size_t time);

    void Reset();

private:
    void Switch();

private:
    size_t intersection_id_;
    Schedule schedule_;
    size_t direction_;
    size_t tick_;
};

} // namespace hashcode