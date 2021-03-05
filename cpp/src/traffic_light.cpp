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

TrafficLight::TrafficLight(Intersection* intersection, const Schedule& schedule)
  : intersection_(intersection),
    schedule_(schedule),
    direction_(0),
    tick_(0)
{
    Reset();
}

size_t TrafficLight::scheduled_streets() const {
    size_t result = 0;
    for (Schedule::const_iterator street_light = schedule_.begin(); street_light != schedule_.end(); street_light++) {
        if (street_light->duration > 0) {
            result++;
        }
    }
    return result;
}

void TrafficLight::Reset() {
    direction_ = 0;
    tick_ = 0;
    scheduled_streets_ = scheduled_streets();
    if (scheduled_streets_ == 0) {
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
    if (++tick_ == street_light.duration) {
        Switch(street_light);
    }
}

StreetLight& TrafficLight::GetNextScheduledStreet() {
    for (size_t i = 0; i < schedule_.size(); i++) {
        StreetLight& street_light = schedule_[direction_];
        if (street_light.duration > 0) {
            return street_light;
        }
        direction_ = (direction_ + 1) % schedule_.size();
    }
    std::ostringstream oss;
    oss << "At least one street light is expected to be scheduled at intersection: " << intersection().id();
    throw std::runtime_error(oss.str());
}

void TrafficLight::Switch(StreetLight& curr_street_light) {
    curr_street_light.street->is_green = false;
    direction_ = (direction_ + 1) % schedule_.size();
    StreetLight& next_street_light = GetNextScheduledStreet();
    next_street_light.street->is_green = true;
    tick_ = 0;
}

} // namespace hashcode