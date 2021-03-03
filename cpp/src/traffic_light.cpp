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
    if (schedule_.empty()) {
        return;
    }
    StreetLight& street_light = schedule_.front();
    if (street_light.duration > 0) {
        street_light.street->is_green = true;
    }
}

void TrafficLight::Reset() {
    for (Schedule::iterator street_light = schedule_.begin(); street_light != schedule_.end();) {
        if (street_light->duration == 0) {
            street_light = schedule_.erase(street_light);
        } else {
            street_light->street->is_green = false;
            street_light++;
        }
    }
    if (schedule_.empty()) {
        return;
    }
    StreetLight& street_light = schedule_.front();
    street_light.street->is_green = true;
    direction_ = 0;
    tick_ = 0;
}

void TrafficLight::Tick(size_t time) {
    if (schedule_.size() <= 1) {
        return;
    }
    StreetLight& street_light = schedule_[direction_];
    if (++tick_ == street_light.duration) {
        Switch();
    }
}

void TrafficLight::Switch() {
    StreetLight& curr_street_light = schedule_[direction_];
    curr_street_light.street->is_green = false;
    direction_ = (direction_ + 1) % schedule_.size();
    StreetLight& next_street_light = schedule_[direction_];
    next_street_light.street->is_green = true;
    tick_ = 0;
}

} // namespace hashcode