#include "solution.hpp"

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

bool GreenLightNameGreater(const GreenLight& lhs, const GreenLight& rhs) {
    return lhs.street->name() > rhs.street->name();
}

TrafficSignaling Solution::GetBestTrafficSignaling() {
    for (std::vector<Car>::iterator car = model_.cars().begin(); car != model_.cars().end(); car++) {
        size_t travel_time = 1; // taking last finish turn into accordance
        for (std::vector<Street*>::iterator it = car->path().begin(); it != car->path().end(); it++) {
            Street* street = *it;
            travel_time += 1 + street->travel_time();
            if (travel_time > model_.simulation_time()) {
                break; // he will never get there...
            }
            travel_time += street->travel_time();
            street->cars_expected++;
        }
    }

    size_t best_score = 0;
    TrafficSignaling signaling;
    for (std::vector<Intersection>::iterator is = model_.intersections().begin(); is != model_.intersections().end(); is++) {
        Schedule schedule;
        for (std::vector<Street*>::iterator it = is->incoming.begin(); it != is->incoming.end(); it++) {
            Street* street = *it;
            if (street->cars_expected == 0) {
                continue;
            }
            schedule.push_back(GreenLight(street, 1)); // TODO
        }
        if (schedule.empty()) {
            continue;
        }
        TrafficLight traffic_light(is->id, schedule);
        signaling.traffic_lights.push_back(traffic_light);
    }

    Simulator simulator(model_);
    size_t bonus = simulator.Run(signaling);
    std::cout << "Best traffic signaling bonus: " << bonus << std::endl;
    return signaling;
}

} // namespace hashcode