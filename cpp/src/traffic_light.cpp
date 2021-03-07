#include "traffic_light.hpp"

#include <algorithm>

#include "fwd.hpp"
#include "intersection.hpp"
#include "model.hpp"
#include "street.hpp"

namespace hashcode
{

ProceedSignal::ProceedSignal(size_t street_id, size_t duration)
  : street_id(street_id),
    duration(duration)
{}

TrafficLight::TrafficLight(size_t intersection_id, const Schedule& schedule)
  : intersection_id_(intersection_id),
    schedule(schedule)
{}

void TrafficLight::Reset(Model& model) {
    clock_ = 0;
    direction_ = 0;
    if (CountScheduledStreets() == 0) {
        return;
    }
    ProceedSignal& proceed_signal = GetNextScheduledProceedSignal();
    model.streets[proceed_signal.street_id].is_green = true;
}

void TrafficLight::Tick(Model& model) {
    if (scheduled_streets_ <= 1) {
        return;
    }
    ProceedSignal& proceed_signal = GetNextScheduledProceedSignal();
    if (++clock_ == proceed_signal.duration) {
        Switch(proceed_signal, model);
    }
}

bool TrafficLight::IncrWorstStreetDuration(Model& model) {
    auto proceed_signal = std::max_element(schedule.begin(), schedule.end(),
        [&] (const ProceedSignal& lhs, const ProceedSignal& rhs) {
            return model.streets[lhs.street_id].time_wasted < model.streets[rhs.street_id].time_wasted;
        }
    );
    if (proceed_signal == schedule.end()) {
        return false;
    }
    ProceedSignal& worst_street = *proceed_signal;
    worst_street.duration++;
    return true;
}

size_t TrafficLight::CountScheduledStreets() {
    size_t result = 0;
    for (const ProceedSignal& proceed_signal : schedule) {
        if (proceed_signal.duration > 0) {
            result++;
        }
    }
    scheduled_streets_ = result;
    return result;
}

ProceedSignal& TrafficLight::GetNextScheduledProceedSignal() {
    for (size_t i = 0; i < schedule.size(); i++) {
        ProceedSignal& proceed_signal = schedule[direction_];
        if (proceed_signal.duration > 0) {
            return proceed_signal;
        }
        direction_ = (direction_ + 1) % schedule.size();
    }
    throw std::runtime_error("At least one proceed signal is expected to be scheduled");
}

void TrafficLight::Switch(ProceedSignal& curr_proceed_signal, Model& model) {
    model.streets[curr_proceed_signal.street_id].is_green = false;
    direction_ = (direction_ + 1) % schedule.size();
    ProceedSignal& next_proceed_signal = GetNextScheduledProceedSignal();
    model.streets[next_proceed_signal.street_id].is_green = true;
    clock_ = 0;
}

} // namespace hashcode