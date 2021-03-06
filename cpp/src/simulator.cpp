#include "simulator.hpp"

#include <algorithm>

#include "fwd.hpp"
#include "model.hpp"
#include "traffic_light.hpp"

namespace hashcode
{

struct CarsOnStreetOrExpectedGreater {
    CarsOnStreetOrExpectedGreater(const Model& model)
      : model_(model)
    {}

    bool operator()(const StreetLight& lhs, const StreetLight& rhs) {
        if (lhs.street->cars.size() == rhs.street->cars.size()) {
            return lhs.street->cars_expected > rhs.street->cars_expected;
        }
        return lhs.street->cars.size() > rhs.street->cars.size();
    }

private:
    const Model& model_;
};

void Simulator::InitializeTrafficLights(Model& model, std::vector<TrafficLight>& traffic_lights) {
    model.CountCarsExpectedOnTheStreets();
    for (Intersection& intersection : model.intersections) {
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
        std::sort(schedule.begin(), schedule.end(), CarsOnStreetOrExpectedGreater(model));
        TrafficLight traffic_light(intersection, schedule);
        traffic_lights.push_back(traffic_light);
    }
}

size_t Simulator::Run(Model& model, std::vector<TrafficLight>& traffic_lights) {
    model.Reset();
    for (TrafficLight& traffic_light : traffic_lights) {
        traffic_light.Reset();
    }
    for (size_t time = 0; time <= model.simulation_time(); time++) {
        for (Car& car : model.cars) {
            car.Tick(time);
        }
        for (TrafficLight& traffic_light : traffic_lights) {
            traffic_light.Tick();
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