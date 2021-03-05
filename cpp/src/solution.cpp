#include "solution.hpp"

#include <algorithm>
#include <cmath>

#include "car.hpp"
#include "fwd.hpp"
#include "model.hpp"
#include "simulator.hpp"
#include "street.hpp"
#include "traffic_signaling.hpp"

namespace hashcode
{

Solution::Solution(Model& model, size_t attempts)
  : model_(model),
    attempts_(attempts)
{}

struct CarsOnStreetOrExpectedGreater {
    bool operator()(const StreetLight& lhs, const StreetLight& rhs) {
        if (lhs.street->cars.size() == rhs.street->cars.size()) {
            return lhs.street->cars_expected > rhs.street->cars_expected;
        }
        return lhs.street->cars.size() > rhs.street->cars.size();
    }
};

struct TimeWastedGreater {
    bool operator()(const StreetLight& lhs, const StreetLight& rhs) {
        return lhs.street->time_wasted > rhs.street->time_wasted;
    }
    bool operator()(const TrafficLight& lhs, const TrafficLight& rhs) {
        return lhs.intersection().time_wasted > rhs.intersection().time_wasted;
    }
};

struct TimeWastedLess {
    bool operator()(const StreetLight& lhs, const StreetLight& rhs) {
        return lhs.street->time_wasted < rhs.street->time_wasted;
    }
};

TrafficSignaling Solution::GetBestTrafficSignaling() {
    Simulator simulator(model_);
    size_t max_bonus = 0;
    TrafficSignaling best_signaling = InitializeWithCarsExpected();
    for (size_t attempt = 0; attempt < attempts_; attempt++) {
        TrafficSignaling signaling = best_signaling;
        std::vector<TrafficLight>& traffic_lights = signaling.traffic_lights;
        max_bonus = simulator.Run(traffic_lights);
        std::cout << "Current traffic signaling bonus: " << max_bonus << std::endl;
        TrafficLight* traffic_light = GetMaxTimeWastedTrafficLight(traffic_lights, attempt);
        if (!traffic_light) {
            std::cout << "Final traffic signaling bonus: " << max_bonus << std::endl;
            return best_signaling;
        }
        bool improved = false;
        Schedule& schedule = traffic_light->schedule();
        for (size_t rotation = 0; rotation < schedule.size(); rotation++) {
            std::rotate(schedule.begin(), schedule.begin() + 1, schedule.end());
            size_t bonus = simulator.Run(traffic_lights);
            if (bonus > max_bonus) {
                best_signaling = signaling;
                max_bonus = bonus;
                improved = true;
                attempt = 0;
            }
        }
        if (improved) {
            continue;
        }
        ExtendMaxTimeWastedStreet(*traffic_light);
        size_t bonus = simulator.Run(traffic_lights);
        if (bonus > max_bonus) {
            best_signaling = signaling;
            max_bonus = bonus;
            attempt = 0;
        }
    }
    std::cout << "Best traffic signaling bonus: " << max_bonus << std::endl;
    return best_signaling;
}

TrafficLight* Solution::GetMaxTimeWastedTrafficLight(std::vector<TrafficLight>& traffic_lights, size_t index) {
    if (index >= traffic_lights.size()) {
        return nullptr;
    }
    std::nth_element(traffic_lights.begin(), traffic_lights.begin() + index, traffic_lights.end(), TimeWastedGreater());
    if (traffic_lights[index].intersection().time_wasted == 0) {
        return nullptr;
    }
    return &traffic_lights[index];
}

bool Solution::ExtendMaxTimeWastedStreet(TrafficLight& traffic_light) {
    Schedule& schedule = traffic_light.schedule();
    Schedule::iterator street_light = std::max_element(schedule.begin(), schedule.end(), TimeWastedLess());
    if (street_light == schedule.end()) {
        return false;
    }
    street_light->duration++;
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
        Schedule schedule;
        for (std::vector<Street*>::iterator it = intersection.streets.begin(); it != intersection.streets.end(); it++) {
            Street* street = *it;
            if (street->cars_expected == 0) {
                continue;
            }
            schedule.push_back(StreetLight(street, 1));
        }
        if (schedule.empty()) {
            continue;
        }
        std::sort(schedule.begin(), schedule.end(), CarsOnStreetOrExpectedGreater());
        TrafficLight traffic_light(&intersection, schedule);
        signaling.traffic_lights.push_back(traffic_light);
    }
    return signaling;
}

} // namespace hashcode