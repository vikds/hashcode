#include "traffic_light.hpp"

#include "fwd.hpp"

#include "street.hpp"

namespace hashcode
{

GreenLight::GreenLight(Street* street, size_t duration)
  : street(street),
    duration(duration)
{}

TrafficLight::TrafficLight(size_t intersection_id, const Schedule& schedule)
  : intersection_id_(intersection_id),
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
    GreenLight& green_light = schedule_[direction_];
    green_light.street->is_green = false;
    schedule_.front().street->is_green = true;
    direction_ = 0;
    tick_ = 0;
}

void TrafficLight::Tick(size_t time) {
    if (schedule_.size() <= 1) {
        return;
    }
    GreenLight& green_light = schedule_[direction_];
    if (++tick_ == green_light.duration) {
        Switch();
    }
}

void TrafficLight::Switch() {
    GreenLight& curr_green_light = schedule_[direction_];
    curr_green_light.street->is_green = false;
    direction_ = (direction_ + 1) % schedule_.size();
    GreenLight& next_green_light = schedule_[direction_];
    next_green_light.street->is_green = true;
    tick_ = 0;
}

} // namespace hashcode