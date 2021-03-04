#pragma once

#include "fwd.hpp"

namespace hashcode
{

class Intersection {
public:
    Intersection(size_t id);

    size_t id() const {
      return id_;
    }

public:
    void Tick(size_t time);

    void Reset();
    double GetAvgCarsJammed(size_t duration) const;
    Street* GetMaxCarExpectedStreet();

public:
    std::vector<Street*> incoming;
    std::vector<Street*> outgoing;
    size_t cars_jammed;
    size_t cars_jammed_sum;
    size_t max_cars_jammed;
    size_t time_wasted;

private:
    size_t id_;
};

} // namespace hashcode