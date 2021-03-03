#include "street.hpp"

#include "fwd.hpp"
#include "intersection.hpp"

namespace hashcode
{

Street::Street(const std::string& name, size_t travel_time, Intersection& intersection)
  : cars_expected(0),
    time_wasted(0),
    car_passed(false),
    is_green(false),
    name_(name),
    travel_time_(travel_time),
    intersection_(intersection)
{}

void Street::Reset() {
    cars.clear();
    cars_expected = 0;
    time_wasted = 0;
    car_passed = false;
    is_green = false;
}

bool Street::IsAllowedToTurn(Car* car) const {
    if (!is_green || car_passed) {
        return false;
    }
    if (cars.empty()) {
        std::ostringstream oss;
        oss << "There are no cars left on the street: " << name_;
        throw std::runtime_error(oss.str());
    }
    return car == cars.front();
}

void Street::TickCosts() {
    intersection_.time_wasted++;
    time_wasted++;
}

} // namespace hashcode