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

TrafficSignaling Solution::GetBestTrafficSignaling() {
    Simulator simulator(model_);

    size_t best_score = 0;
    TrafficSignaling best_traffic_signaling;

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

    TrafficSignaling traffic_signaling;
    for (std::vector<Intersection>::iterator is = model_.intersections().begin(); is != model_.intersections().end(); is++) {
        Schedule schedule;
        for (std::vector<Street*>::iterator street = is->incoming.begin(); street != is->incoming.end(); street++) {
            Street* street_ptr = *street;
            if (street_ptr->cars_expected == 0) {
                continue;
            }
            schedule.push_back(GreenLight(street_ptr, 1)); // TODO
        }
        if (schedule.empty()) {
            continue;
        }
        traffic_signaling.traffic_lights.push_back(TrafficLight(is->id, schedule));
    }

    size_t bonus = simulator.Run(traffic_signaling);
    std::cout << "Bonus: " << bonus << std::endl;
    return traffic_signaling;
}

} // namespace hashcode