#include "street.hpp"

#include "fwd.hpp"
#include "intersection.hpp"

namespace hashcode
{

Street::Street(Intersection& intersection, const std::string& name, size_t travel_time)
  : intersection(intersection),
    name_(name),
    travel_time_(travel_time),
    cars_expected(0),
    time_wasted(0),
    car_passed(false),
    is_green(false)
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
    intersection.time_wasted++;
    time_wasted++;
}

bool Street::IsAllowedToTurn(Car* car) const {
    if (!is_green || car_passed) {
        return false;
    }
    return car == cars.front();
}

} // namespace hashcode