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
    schedule_.front().street->is_green = true;
}

void TrafficLight::Reset() {
    if (schedule_.empty()) {
        return;
    }
    for (Schedule::iterator street_light = schedule_.begin(); street_light != schedule_.end(); street_light++) {
        street_light->street->is_green = false;
    }
    schedule_.front().street->is_green = true;
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