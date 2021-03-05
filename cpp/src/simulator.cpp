#include "simulator.hpp"

#include "fwd.hpp"
#include "model.hpp"
#include "traffic_light.hpp"
#include "traffic_signaling.hpp"

namespace hashcode
{

Simulator::Simulator(Model& model)
  : model_(model)
{}

size_t Simulator::Run(std::vector<TrafficLight>& traffic_lights) {
    model_.Reset();
    for (std::vector<TrafficLight>::iterator traffic_light = traffic_lights.begin(); traffic_light != traffic_lights.end(); traffic_light++) {
        traffic_light->Reset();
    }
    for (size_t time = 0; time <= model_.simulation_time(); time++) {
        for (std::vector<Car>::iterator car = model_.cars().begin(); car != model_.cars().end(); car++) {
            car->Tick(time);
        }
        for (std::vector<TrafficLight>::iterator traffic_light = traffic_lights.begin(); traffic_light != traffic_lights.end(); traffic_light++) {
            traffic_light->Tick();
        }
        for (std::vector<Street>::iterator street = model_.streets().begin(); street != model_.streets().end(); street++) {
            street->Tick();
        }
    }
    size_t score = 0;
    for (std::vector<Car>::iterator car = model_.cars().begin(); car != model_.cars().end(); car++) {
        if (!car->HasFinished()) {
            continue;
        }
        size_t bonus = model_.finish_bonus() + (model_.simulation_time() - car->finish_time());
        score += bonus;
    }
    return score;
}

} // namespace hashcode