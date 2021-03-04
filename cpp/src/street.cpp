#include "street.hpp"

#include "fwd.hpp"
#include "intersection.hpp"

namespace hashcode
{

Street::Street(const std::string& name, size_t travel_time, Intersection& intersection)
  : cars_expected(0),
    cars_jammed(0),
    cars_jammed_sum(0),
    max_cars_jammed(0),
    time_wasted(0),
    car_passed(false),
    is_green(false),
    name_(name),
    travel_time_(travel_time),
    intersection_(intersection)
{}

void Street::Tick(size_t time) {
    car_passed = false;
    cars_jammed_sum += cars_jammed;
    max_cars_jammed = std::max(max_cars_jammed, cars_jammed);
    cars_jammed = 0;
}

void Street::Reset() {
    cars.clear();
    cars_expected = 0;
    cars_jammed = 0;
    cars_jammed_sum = 0;
    max_cars_jammed = 0;
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

double Street::GetAvgCarsJammed(size_t duration) const {
    return static_cast<double>(cars_jammed_sum) / duration;
}

void Street::TickLoss() {
    intersection_.cars_jammed++;
    cars_jammed++;
    time_wasted++;
}

} // namespace hashcode