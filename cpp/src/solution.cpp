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

bool SortByDurationDesc(const StreetLight& lhs, const StreetLight& rhs) {
    return lhs.duration > rhs.duration;
}

bool SortByCarExpectedDesc(const StreetLight& lhs, const StreetLight& rhs) {
    return lhs.street->cars_expected > rhs.street->cars_expected;
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
    std::sort(schedule.begin(), schedule.end(), SortByDurationDesc);
    return true;
}

void Solution::CountCarsExpectedOnTheStreets() {
    for (std::vector<Car>::iterator car = model_.cars().begin(); car != model_.cars().end(); car++) {
        if (car->path().empty()) {
            continue;
        }
        size_t travel_time = 0;
        for (std::vector<Street*>::iterator it = car->path().begin(); it != car->path().end(); it++) {
            if (travel_time > model_.simulation_time()) {
                break;
            }
            Street* street = *it;
            if (it != car->path().begin()) {
                travel_time += street->travel_time();
            }
            street->cars_expected++;
        }
    }
}

TrafficSignaling Solution::InitializeWithCarsExpected() {
    CountCarsExpectedOnTheStreets();
    TrafficSignaling signaling;
    for (std::vector<Intersection>::iterator is = model_.intersections().begin(); is != model_.intersections().end(); is++) {
        Intersection& intersection = *is;
        Street* street = intersection.GetMaxCarExpectedStreet();
        if (!street) {
            continue;
        }
        Schedule schedule;
        for (std::vector<Street*>::iterator it = intersection.incoming.begin(); it != intersection.incoming.end(); it++) {
            Street* street = *it;
            if (street->cars_expected == 0) {
                continue;
            }
            schedule.push_back(StreetLight(street, 1));
        }
        if (schedule.empty()) {
            continue;
        }
        std::sort(schedule.begin(), schedule.end(), SortByCarExpectedDesc);
        TrafficLight traffic_light(&intersection, schedule);
        signaling.traffic_lights.push_back(traffic_light);
    }
    return signaling;
}

} // namespace hashcode