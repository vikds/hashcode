#include "traffic_light.hpp"

#include "fwd.hpp"
#include "intersection.hpp"
#include "street.hpp"

namespace hashcode
{

StreetLight::StreetLight(Street* street, size_t duration)
  : street(street),
    duration(duration)
{}

TrafficLight::TrafficLight(const Intersection& intersection, const Schedule& schedule)
  : intersection(&intersection),
    schedule(schedule)
{
    Reset();
}

void TrafficLight::Reset() {
    clock_ = 0;
    direction_ = 0;
    if (CountScheduledStreets() == 0) {
        return;
    }
    StreetLight& street_light = GetNextScheduledStreet();
    street_light.street->is_green = true;
}

void TrafficLight::Tick() {
    if (scheduled_streets_ <= 1) {
        return;
    }
    StreetLight& street_light = GetNextScheduledStreet();
    if (++clock_ == street_light.duration) {
        Switch(street_light);
    }
}

size_t TrafficLight::CountScheduledStreets() {
    size_t result = 0;
    for (const StreetLight& street_light : schedule) {
        if (street_light.duration > 0) {
            result++;
        }
    }
    scheduled_streets_ = result;
    return result;
}

StreetLight& TrafficLight::GetNextScheduledStreet() {
    for (size_t i = 0; i < schedule.size(); i++) {
        StreetLight& street_light = schedule[direction_];
        if (street_light.duration > 0) {
            return street_light;
        }
        direction_ = (direction_ + 1) % schedule.size();
    }
    throw std::runtime_error("At least one street light is expected to be scheduled");
}

void TrafficLight::Switch(StreetLight& curr_street_light) {
    curr_street_light.street->is_green = false;
    direction_ = (direction_ + 1) % schedule.size();
    StreetLight& next_street_light = GetNextScheduledStreet();
    next_street_light.street->is_green = true;
    clock_ = 0;
}

} // namespace hashcode