#include "solution.hpp"

#include <cmath>

#include "car.hpp"
#include "fwd.hpp"
#include "model.hpp"
#include "street.hpp"
#include "simulator.hpp"
#include "street.hpp"
#include "traffic_signaling.hpp"

namespace hashcode
{

Solution::Solution(Model& model, size_t attempts)
  : model_(model),
    attempts_(attempts)
{}

bool StreetLightDurationGreater(const StreetLight& lhs, const StreetLight& rhs) {
    return lhs.duration > rhs.duration;
}

TrafficSignaling Solution::GetBestTrafficSignaling() {
    Simulator simulator(model_);
    TrafficSignaling signaling = InitializeWithCarsExpected();
    TrafficSignaling best_signaling = signaling;
    size_t max_bonus = simulator.Run(signaling);
    std::cout << "Initial signaling bonus: " << max_bonus << std::endl;
    for (size_t attempt = 0; attempt < attempts_; attempt++) {
        std::vector<TrafficLight>& traffic_lights = signaling.traffic_lights;
        size_t max_time_wasted = 0;
        for (std::vector<TrafficLight>::iterator traffic_light = traffic_lights.begin(); traffic_light != traffic_lights.end(); traffic_light++) {
            max_time_wasted = std::max(max_time_wasted, traffic_light->intersection().time_wasted);
        }
        if (max_time_wasted == 0) {
            break;
        }
        for (std::vector<TrafficLight>::iterator traffic_light = traffic_lights.begin(); traffic_light != traffic_lights.end(); traffic_light++) {
            if (traffic_light->intersection().time_wasted == max_time_wasted) {
                ChangeTrafficLightSchedule(*traffic_light);
            }
        }
        size_t bonus = simulator.Run(signaling);
        std::cout << "Intermediate signaling bonus: " << bonus << std::endl;
        if (bonus > max_bonus) {
            best_signaling = signaling;
            max_bonus = bonus;
            attempt = 0;
        }
    }
    std::cout << "Best traffic signaling bonus: " << max_bonus << std::endl;
    return best_signaling;
}

bool Solution::ChangeTrafficLightSchedule(TrafficLight& traffic_light) {
    size_t max_time_wasted = 0;
    Schedule& schedule = traffic_light.schedule();
    for (Schedule::iterator street_light = schedule.begin(); street_light != schedule.end(); street_light++) {
        max_time_wasted = std::max(max_time_wasted, street_light->street->time_wasted);
    }
    if (max_time_wasted == 0) {
        return false;
    }
    for (Schedule::iterator street_light = schedule.begin(); street_light != schedule.end(); street_light++) {
        if (street_light->street->time_wasted == max_time_wasted) {
            street_light->duration++;
        }
    }
    std::sort(schedule.begin(), schedule.end(), StreetLightDurationGreater);
    return true;
}

void Solution::CountExpectedCarsOnTheStreets() {
    for (std::vector<Car>::iterator car = model_.cars().begin(); car != model_.cars().end(); car++) {
        if (car->path().empty()) {
            continue;
        }
        size_t travel_time = 1;
        for (std::vector<Street*>::iterator it = car->path().begin(); it != car->path().end(); it++) {
            Street* street = *it;
            if (travel_time > model_.simulation_time()) {
                break; // he will never get there...
            }
            if (it != car->path().begin()) {
                travel_time += street->travel_time();
            }
            street->cars_expected++;
        }
    }
}

TrafficSignaling Solution::InitializeWithCarsExpected() {
    CountExpectedCarsOnTheStreets();
    TrafficSignaling signaling;
    for (std::vector<Intersection>::iterator is = model_.intersections().begin(); is != model_.intersections().end(); is++) {
        Schedule schedule;
        for (std::vector<Street*>::iterator it = is->incoming.begin(); it != is->incoming.end(); it++) {
            Street* street = *it;
            if (street->cars_expected == 0) {
                continue;
            }
            schedule.push_back(StreetLight(street, street->cars_expected));
        }
        if (schedule.empty()) {
            continue;
        }
        std::sort(schedule.begin(), schedule.end(), StreetLightDurationGreater);
        for (Schedule::iterator street_light = schedule.begin(); street_light != schedule.end(); street_light++) {
            street_light->duration = 1;
        }
        TrafficLight traffic_light(&*is, schedule);
        signaling.traffic_lights.push_back(traffic_light);
    }
    return signaling;
}

} // namespace hashcode