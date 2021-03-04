#include "intersection.hpp"

#include "fwd.hpp"
#include "street.hpp"

namespace hashcode
{

Intersection::Intersection(size_t id)
  : cars_jammed(0),
    cars_jammed_sum(0),
    max_cars_jammed(0),
    time_wasted(0),
    id_(id)
{}

void Intersection::Tick(size_t time) {
    cars_jammed_sum += cars_jammed;
    max_cars_jammed = std::max(max_cars_jammed, cars_jammed);
    cars_jammed = 0;
}

void Intersection::Reset() {
    cars_jammed = 0;
    cars_jammed_sum = 0;
    max_cars_jammed = 0;
    time_wasted = 0;
}

double Intersection::GetAvgCarsJammed(size_t duration) const {
    return static_cast<double>(cars_jammed_sum) / duration;
}

Street* Intersection::GetMaxCarExpectedStreet() {
    Street* result = nullptr;
    size_t max_car_expected = 0;
    for (std::vector<Street*>::iterator it = incoming.begin(); it != incoming.end(); it++) {
        Street* street = *it;
        if (street->cars_expected > max_car_expected) {
            max_car_expected = street->cars_expected;
            result = street;
        }
    }
    return result;
}

} // namespace hashcode