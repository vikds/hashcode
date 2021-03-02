#include "street.hpp"

#include "fwd.hpp"

namespace hashcode
{

Street::Street(const std::string& name, size_t travel_time)
  : name_(name),
    travel_time_(travel_time),
    cars_expected(0),
    time_wasted(0),
    is_green(false),
    ticked(false)
{}

void Street::Reset() {
    cars.clear();
    cars_expected = 0;
    time_wasted = 0;
    is_green = false;
    ticked = false;
}

} // namespace hashcode