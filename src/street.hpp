#pragma once

#include "fwd.hpp"

#include <deque>

namespace hashcode
{

class Street {
public:
    Street(Intersection& intersection, const std::string& name, size_t travel_time);

    const std::string& name() const {
        return name_;
    }
    size_t travel_time() const {
        return travel_time_;
    }

public:
    void Reset();
    void Tick();
    void AddTimeWasted();
    bool IsAllowedToTurn(Car* car) const;

public:
    Intersection& intersection;
    std::deque<Car*> cars;
    size_t cars_expected = 0;
    size_t time_wasted = 0;
    bool car_passed = false;
    bool is_green = false;

private:
    const std::string& name_;
    size_t travel_time_;
};

} // namespace hashcode