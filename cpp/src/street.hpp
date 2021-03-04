#pragma once

#include "fwd.hpp"

#include <deque>

namespace hashcode
{

class Street {
public:
    Street(const std::string& name, size_t travel_time, Intersection& intersection);

    const std::string& name() const {
        return name_;
    }
    size_t travel_time() const {
        return travel_time_;
    }
    Intersection& intersection() {
        return intersection_;
    }

public:
    void Reset();

    void Tick(size_t);

    bool IsAllowedToTurn(Car* car) const;

    double GetAvgCarsJammed(size_t duration) const;

    void TickLoss();

public: // resetable
    std::deque<Car*> cars;
    size_t cars_expected;
    size_t cars_jammed;
    size_t cars_jammed_sum;
    size_t max_cars_jammed;
    size_t time_wasted;
    bool car_passed;
    bool is_green;

private:
    std::string name_;
    size_t travel_time_;
    Intersection& intersection_;
};

} // namespace hashcode