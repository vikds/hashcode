#include "street.hpp"

#include "fwd.hpp"
#include "intersection.hpp"

namespace hashcode
{

Street::Street(size_t id, Intersection& intersection, size_t travel_time)
  : id_(id),
    intersection_(intersection),
    travel_time_(travel_time)
{}

void Street::Reset() {
    cars.clear();
    cars_expected = 0;
    time_wasted = 0;
    car_passed = false;
    is_green = false;
}

void Street::Tick() {
    car_passed = false;
}

void Street::AddTimeWasted() {
    intersection_.time_wasted++;
    time_wasted++;
}

bool Street::IsAllowedToTurn(Car* car) const {
    if (!is_green || car_passed) {
        return false;
    }
    return car == cars.front();
}

} // namespace hashcode