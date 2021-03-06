#pragma once

#include "fwd.hpp"

namespace hashcode
{

class Simulator {
public:
    static void InitializeTrafficLights(Model& model, std::vector<TrafficLight>& traffic_lights);
    static size_t Run(Model& model, std::vector<TrafficLight>& traffic_lights);
};

} // namespace hashcode