#include "simulator.hpp"

#include <algorithm>

#include "fwd.hpp"
#include "model.hpp"
#include "timer.hpp"
#include "traffic_light.hpp"

namespace hashcode
{

struct CarsOnStreetOrExpectedGreater {
    CarsOnStreetOrExpectedGreater(const Model& model)
      : model_(model)
    {}

    bool operator()(const ProceedSignal& lhs, const ProceedSignal& rhs) {
        if (model_.streets[lhs.street_id].cars.size() == model_.streets[rhs.street_id].cars.size()) {
            return model_.streets[lhs.street_id].cars_expected > model_.streets[rhs.street_id].cars_expected;
        }
        return model_.streets[lhs.street_id].cars.size() > model_.streets[rhs.street_id].cars.size();
    }

private:
    const Model& model_;
};

void Simulator::InitializeTrafficLights(Model& model, Signaling& signaling) {
    Timer init_timer("Simulator initalizing");
    model.CountCarsExpectedOnTheStreets();
    for (Intersection& intersection : model.intersections) {
        Schedule schedule;
        for (Street& street : intersection.streets) {
            if (street.cars_expected == 0) {
                continue;
            }
            schedule.push_back(ProceedSignal(street.id(), 1));
        }
        if (schedule.empty()) {
            continue;
        }
        std::sort(schedule.begin(), schedule.end(), CarsOnStreetOrExpectedGreater(model));
        TrafficLight traffic_light(intersection.id(), schedule);
        signaling.traffic_lights.push_back(traffic_light);
    }
}

size_t Simulator::Run(Model& model, Signaling& signaling, const std::string& info) {
    std::string message = std::string("Simulator run");
    if (!info.empty()) {
        message += ": " + info;
    }
    Timer rotation_timer(message);
    model.Reset();
    for (TrafficLight& traffic_light : signaling.traffic_lights) {
        traffic_light.Reset(model);
    }
    for (size_t time = 0; time <= model.simulation_time(); time++) {
        for (Car& car : model.cars) {
            car.Tick(time);
        }
        for (TrafficLight& traffic_light : signaling.traffic_lights) {
            traffic_light.Tick(model);
        }
        for (Street& street : model.streets) {
            street.Tick();
        }
    }
    size_t score = 0;
    for (Car& car : model.cars) {
        if (!car.has_finished()) {
            continue;
        }
        size_t bonus = model.finish_bonus() + (model.simulation_time() - car.finish_time());
        score += bonus;
    }
    return score;
}

} // namespace hashcode