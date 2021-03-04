#include "intersection.hpp"

#include "fwd.hpp"
#include "street.hpp"

namespace hashcode
{

Intersection::Intersection(size_t id)
  : time_wasted(0),
    id_(id)
{}

void Intersection::Tick(size_t time) {
}

void Intersection::Reset() {
    time_wasted = 0;
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