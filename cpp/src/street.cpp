#include "street.hpp"

#include "fwd.hpp"
#include "intersection.hpp"

namespace hashcode
{

Street::Street(const std::string& name, size_t travel_time, Intersection& intersection)
  : cars_expected(0),
    time_wasted(0),
    is_green(false),
    ticked(false),
    name_(name),
    travel_time_(travel_time),
    intersection_(intersection)
{}

void Street::Reset() {
    cars.clear();
    cars_expected = 0;
    time_wasted = 0;
    is_green = false;
    ticked = false;
}

bool Street::IsAllowedToTurn(Car* car) const {
    if (!is_green || ticked) {
        return false;
    }
    if (cars.empty()) {
        std::ostringstream oss;
        oss << "Cars are not placed on the street: " << name_;
        throw std::runtime_error(oss.str());
    }
    return car == cars.front();
}

void Street::TickCosts() {
    intersection_.time_wasted++;
    time_wasted++;
}

} // namespace hashcode