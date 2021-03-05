#include "simulator.hpp"

#include <algorithm>

#include "fwd.hpp"
#include "model.hpp"
#include "traffic_light.hpp"

namespace hashcode
{

Simulator::Simulator(Model& model)
  : model_(model)
{}

struct CarsOnStreetOrExpectedGreater {
    bool operator()(const StreetLight& lhs, const StreetLight& rhs) {
        if (lhs.street->cars.size() == rhs.street->cars.size()) {
            return lhs.street->cars_expected > rhs.street->cars_expected;
        }
        return lhs.street->cars.size() > rhs.street->cars.size();
    }
};

void Simulator::InitializeWithCarsExpected(std::vector<TrafficLight>& traffic_lights) {
    model_.CountCarsExpectedOnTheStreets();
    for (Intersection& intersection : model_.intersections) {
        Schedule schedule;
        for (Street* street : intersection.streets) {
            if (street->cars_expected == 0) {
                continue;
            }
            schedule.push_back(StreetLight(street, 1));
        }
        if (schedule.empty()) {
            continue;
        }
        std::sort(schedule.begin(), schedule.end(), CarsOnStreetOrExpectedGreater());
        TrafficLight traffic_light(intersection, schedule);
        traffic_lights.push_back(traffic_light);
    }
}

size_t Simulator::Run(std::vector<TrafficLight>& traffic_lights) {
    model_.Reset();
    for (TrafficLight& traffic_light : traffic_lights) {
        traffic_light.Reset();
    }
    for (size_t time = 0; time <= model_.simulation_time(); time++) {
        for (Car& car : model_.cars) {
            car.Tick(time);
        }
        for (TrafficLight& traffic_light : traffic_lights) {
            traffic_light.Tick();
        }
        for (Street& street : model_.streets) {
            street.Tick();
        }
    }
    size_t score = 0;
    for (Car& car : model_.cars) {
        if (!car.has_finished()) {
            continue;
        }
        size_t bonus = model_.finish_bonus() + (model_.simulation_time() - car.finish_time());
        score += bonus;
    }
    return score;
}

} // namespace hashcode