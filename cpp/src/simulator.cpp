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

size_t Simulator::Run(TrafficSignaling& traffic_signaling) {
    for (size_t time = 0; time < model_.simulation_time(); time++) {
        for (std::vector<Car>::iterator car = model_.cars().begin(); car != model_.cars().end(); car++) {
            car->Tick(time);
        }
        std::vector<TrafficLight>& traffic_lights = traffic_signaling.traffic_lights;
        for (std::vector<TrafficLight>::iterator light = traffic_lights.begin(); light != traffic_lights.end(); light++) {
            light->Tick(time);
        }
    }
    size_t score = 0;
    for (std::vector<Car>::iterator car = model_.cars().begin(); car != model_.cars().end(); car++) {
        if (!car->HasFinished()) {
            continue;
        }
        score += model_.finish_bonus() + (model_.simulation_time() - car->finish_time());
    }
    return score;
}

void Simulator::Reset() {
    model_.Reset();
}

} // namespace hashcode