#pragma once

#include "fwd.hpp"

#include <deque>

namespace hashcode
{

class Street {
public:
    Street(size_t id, Intersection& intersection, size_t travel_time);

    size_t id() const {
        return id_;
    }
    size_t travel_time() const {
        return travel_time_;
    }

public:
    void Reset();
    void Tick();
    void AddTimeWasted();
    bool IsAllowedToTurn(CarRef car) const;

public:
    std::deque<CarRef> cars;
    size_t cars_expected = 0;
    size_t time_wasted = 0;
    bool car_passed = false;
    bool is_green = false;

private:
    size_t id_;
    Intersection& intersection_;
    size_t travel_time_;
};

} // namespace hashcode