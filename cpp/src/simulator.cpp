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

size_t Simulator::Run(TrafficSignaling& signaling) {
    for (size_t time = 0; time <= model_.simulation_time(); time++) {
        for (std::vector<Car>::iterator car = model_.cars().begin(); car != model_.cars().end(); car++) {
            car->Tick(time);
        }
        for (std::vector<TrafficLight>::iterator light = signaling.traffic_lights.begin(); light != signaling.traffic_lights.end(); light++) {
            light->Tick(time);
        }
        for (std::vector<Street>::iterator street = model_.streets().begin(); street != model_.streets().end(); street++) {
            street->ticked = false;
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

void Simulator::Reset() {
    model_.Reset();
}

} // namespace hashcode