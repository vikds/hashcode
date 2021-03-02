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

Solution::Solution(Model& model)
  : model_(model)
{}

bool GreenLightDurationGreater(const GreenLight& lhs, const GreenLight& rhs) {
    return lhs.duration > rhs.duration;
}

TrafficSignaling Solution::GetBestTrafficSignaling() {
    Simulator simulator(model_);
    TrafficSignaling signaling = InitializeWithCarsExpected();
    TrafficSignaling best_signaling = signaling;
    size_t max_bonus = simulator.Run(signaling);
    std::cout << "Initialized signaling bonus: " << max_bonus << std::endl;
    for (size_t attempt = 0; attempt < 5; attempt++) {
        for (std::vector<TrafficLight>::iterator light = signaling.traffic_lights.begin(); light != signaling.traffic_lights.end(); light++) {
            size_t max_time_wasted = 0;
            for (Schedule::iterator green_light = light->schedule().begin(); green_light != light->schedule().end(); green_light++) {
                max_time_wasted = std::max(max_time_wasted, green_light->street->time_wasted);
            }
            for (Schedule::iterator green_light = light->schedule().begin(); green_light != light->schedule().end(); green_light++) {
                if (green_light->street->time_wasted == max_time_wasted) {
                    green_light->duration++;
                }
            }
            std::sort(light->schedule().begin(), light->schedule().end(), GreenLightDurationGreater);
            light->Reset();
        }
        size_t bonus = simulator.Run(signaling);
        std::cout << "Intermediate signaling bonus: " << bonus << std::endl;
        if (bonus > max_bonus) {
            best_signaling = signaling;
            max_bonus = bonus;
            attempt = 0;
        }
        simulator.Reset();
    }
    std::cout << "Best traffic signaling bonus: " << max_bonus << std::endl;
    return best_signaling;
}

TrafficSignaling Solution::InitializeWithCarsExpected() {
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

    TrafficSignaling signaling;
    for (std::vector<Intersection>::iterator is = model_.intersections().begin(); is != model_.intersections().end(); is++) {
        Schedule schedule;
        for (std::vector<Street*>::iterator it = is->incoming.begin(); it != is->incoming.end(); it++) {
            Street* street = *it;
            if (street->cars_expected == 0) {
                continue;
            }
            schedule.push_back(GreenLight(street, street->cars_expected));
        }
        if (schedule.empty()) {
            continue;
        }
        std::sort(schedule.begin(), schedule.end(), GreenLightDurationGreater);
        size_t min_cars_expected = schedule.back().duration;
        for (Schedule::iterator green_light = schedule.begin(); green_light != schedule.end(); green_light++) {
            green_light->duration = std::round(static_cast<double>(green_light->duration) / min_cars_expected);
        }
        TrafficLight traffic_light(is->id, schedule);
        signaling.traffic_lights.push_back(traffic_light);
    }
    return signaling;
}

} // namespace hashcode