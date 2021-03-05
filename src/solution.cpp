#include "solution.hpp"

#include <algorithm>

#include "car.hpp"
#include "fwd.hpp"
#include "model.hpp"
#include "result.hpp"
#include "simulator.hpp"
#include "street.hpp"

namespace hashcode
{

Solution::Solution(Model& model, size_t attempts)
  : model_(model),
    attempts_(attempts)
{}

Result Solution::GetBestResult() {
    Simulator simulator(model_);
    size_t bonus;
    size_t max_bonus = 0;
    Result curr;
    Result best;
    simulator.InitializeWithCarsExpected(best.traffic_lights);
    for (size_t attempt = 0; attempt < attempts_; attempt++) {
        curr = best;
        max_bonus = simulator.Run(curr.traffic_lights);
        std::cout << "Current traffic signaling bonus: " << max_bonus << std::endl;
        TrafficLight* traffic_light = GetMaxTimeWastedTrafficLight(curr.traffic_lights, attempt);
        if (!traffic_light) {
            std::cout << "Final traffic signaling bonus: " << max_bonus << std::endl;
            return best;
        }
        bool improved = false;
        Schedule& schedule = traffic_light->schedule;
        for (size_t rotation = 0; rotation < schedule.size(); rotation++) {
            std::rotate(schedule.begin(), schedule.begin() + 1, schedule.end());
            bonus = simulator.Run(curr.traffic_lights);
            if (bonus > max_bonus) {
                best = curr;
                max_bonus = bonus;
                improved = true;
                attempt = 0;
            }
        }
        if (improved) {
            continue;
        }
        ExtendMaxTimeWastedStreet(*traffic_light);
        bonus = simulator.Run(curr.traffic_lights);
        if (bonus > max_bonus) {
            best = curr;
            max_bonus = bonus;
            attempt = 0;
        }
    }
    std::cout << "Best traffic signaling bonus: " << max_bonus << std::endl;
    return best;
}

TrafficLight* Solution::GetMaxTimeWastedTrafficLight(std::vector<TrafficLight>& traffic_lights, size_t index) {
    if (index >= traffic_lights.size()) {
        return nullptr;
    }
    std::nth_element(traffic_lights.begin(), traffic_lights.begin() + index, traffic_lights.end(),
        [&] (const TrafficLight& lhs, const TrafficLight& rhs) {
            return lhs.intersection->time_wasted > rhs.intersection->time_wasted;
        }
    );
    if (traffic_lights[index].intersection->time_wasted == 0) {
        return nullptr;
    }
    return &traffic_lights[index];
}

bool Solution::ExtendMaxTimeWastedStreet(TrafficLight& traffic_light) {
    auto street_light = std::max_element(traffic_light.schedule.begin(), traffic_light.schedule.end(),
        [&] (const StreetLight& lhs, const StreetLight& rhs) {
            return lhs.street->time_wasted < rhs.street->time_wasted;
        }
    );
    if (street_light == traffic_light.schedule.end()) {
        return false;
    }
    street_light->duration++;
    return true;
}

} // namespace hashcode