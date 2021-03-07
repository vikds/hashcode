#pragma once

#include "fwd.hpp"

namespace hashcode
{

struct ProceedSignal {
    ProceedSignal(size_t street_id, size_t duration);

    size_t street_id;
    size_t duration;
};

class TrafficLight {
public:
    TrafficLight(size_t intersection_id, const Schedule& schedule);

    size_t intersection_id() const {
        return intersection_id_;
    }
    size_t scheduled_streets() const {
        return scheduled_streets_;
    }

public:
    void Reset(Model& model);
    void Tick(Model& model);
    bool IncrWorstStreetDuration(Model& model);

private:
    size_t CountScheduledStreets();
    ProceedSignal& GetNextScheduledProceedSignal();
    void Switch(ProceedSignal& curr_proceed_signal, Model& model);

public:
    Schedule schedule;

private:
    size_t intersection_id_;
    size_t clock_ = 0;
    size_t direction_ = 0;
    size_t scheduled_streets_;
};

} // namespace hashcode