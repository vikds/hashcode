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

bool SortByMaxCarsJammedDesc(const StreetLight& lhs, const StreetLight& rhs) {
    return lhs.street->max_cars_jammed > rhs.street->max_cars_jammed;
}

struct SortByAvgCarsJammedDesc {
    SortByAvgCarsJammedDesc(size_t duration)
      : duration_(duration)
    {}

    bool operator()(const StreetLight& lhs, const StreetLight& rhs) {
        return lhs.street->GetAvgCarsJammed(duration_) < rhs.street->GetAvgCarsJammed(duration_);
    }

private:
    size_t duration_;
};

TrafficSignaling Solution::GetBestTrafficSignaling() {
    Simulator simulator(model_);
    TrafficSignaling signaling = InitializeWithCarsExpected();
    TrafficSignaling best_signaling = signaling;
    size_t max_bonus = simulator.Run(signaling);
    std::cout << "Initial signaling bonus: " << max_bonus << std::endl;
    for (size_t attempt = 0; attempt < attempts_; attempt++) {
        std::vector<TrafficLight>& traffic_lights = signaling.traffic_lights;
        TrafficLight* traffic_light = GetAvgCarsJammedTrafficLight(traffic_lights);
        if (!traffic_light) {
            break;
        }
        OpenAvgCarsJammedStreet(*traffic_light);
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

TrafficLight* Solution::GetMaxCarsJammedTrafficLight(std::vector<TrafficLight>& traffic_lights) {
    size_t max_cars_jammed = 0;
    for (std::vector<TrafficLight>::iterator traffic_light = traffic_lights.begin(); traffic_light != traffic_lights.end(); traffic_light++) {
        max_cars_jammed = std::max(max_cars_jammed, traffic_light->intersection().max_cars_jammed);
    }
    if (max_cars_jammed == 0) {
        return nullptr;
    }
    for (std::vector<TrafficLight>::iterator traffic_light = traffic_lights.begin(); traffic_light != traffic_lights.end(); traffic_light++) {
        if (traffic_light->intersection().max_cars_jammed == max_cars_jammed) {
            return &*traffic_light;
        }
    }
    return nullptr;
}

TrafficLight* Solution::GetAvgCarsJammedTrafficLight(std::vector<TrafficLight>& traffic_lights) {
    double max_cars_jammed = 0;
    for (std::vector<TrafficLight>::iterator traffic_light = traffic_lights.begin(); traffic_light != traffic_lights.end(); traffic_light++) {
        max_cars_jammed = std::max(max_cars_jammed, traffic_light->intersection().GetAvgCarsJammed(model_.simulation_time()));
    }
    if (max_cars_jammed == 0.f) {
        return nullptr;
    }
    for (std::vector<TrafficLight>::iterator traffic_light = traffic_lights.begin(); traffic_light != traffic_lights.end(); traffic_light++) {
        if (traffic_light->intersection().GetAvgCarsJammed(model_.simulation_time()) == max_cars_jammed) {
            return &*traffic_light;
        }
    }
    return nullptr;
}

bool Solution::OpenAvgCarsJammedStreet(TrafficLight& traffic_light) {
    double max_cars_jammed = 0;
    Schedule& schedule = traffic_light.schedule();
    for (Schedule::iterator street_light = schedule.begin(); street_light != schedule.end(); street_light++) {
        max_cars_jammed = std::max(max_cars_jammed, street_light->street->GetAvgCarsJammed(model_.simulation_time()));
    }
    if (max_cars_jammed == 0) {
        return false;
    }
    for (Schedule::iterator street_light = schedule.begin(); street_light != schedule.end(); street_light++) {
        if (street_light->street->GetAvgCarsJammed(model_.simulation_time()) == max_cars_jammed) {
            street_light->duration++;
            break;
        }
    }
    std::sort(schedule.begin(), schedule.end(), SortByAvgCarsJammedDesc(model_.simulation_time()));
    return true;
}

bool Solution::OpenMaxCarsJammedStreet(TrafficLight& traffic_light) {
    size_t max_cars_jammed = 0;
    Schedule& schedule = traffic_light.schedule();
    for (Schedule::iterator street_light = schedule.begin(); street_light != schedule.end(); street_light++) {
        max_cars_jammed = std::max(max_cars_jammed, street_light->street->max_cars_jammed);
    }
    if (max_cars_jammed == 0) {
        return false;
    }
    for (Schedule::iterator street_light = schedule.begin(); street_light != schedule.end(); street_light++) {
        if (street_light->street->max_cars_jammed == max_cars_jammed) {
            street_light->duration++;
            break;
        }
    }
    std::sort(schedule.begin(), schedule.end(), SortByMaxCarsJammedDesc);
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