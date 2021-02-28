#include "simulator.hpp"

#include "fwd.hpp"
#include "model.hpp"

namespace hashcode
{

size_t Simulator::Run(Model& model) {
    for (size_t time = 0; time < model.simulation_time(); time++) {
        for (std::vector<Car>::iterator car = model.cars().begin(); car != model.cars().end(); car++) {
            car->Tick();
        }
        for (std::vector<TrafficLight>::iterator lights = model.traffic_lights().begin(); lights != model.traffic_lights().end(); lights++) {
            lights->Tick();
        }
    }
    return 0; // TODO;
}

} // namespace hashcode