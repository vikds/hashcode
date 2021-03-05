#pragma once

#include "fwd.hpp"

namespace hashcode
{

class Simulator {
public:
    Simulator(Model& model);

    void InitializeWithCarsExpected(std::vector<TrafficLight>& traffic_lights);
    size_t Run(std::vector<TrafficLight>& traffic_lights);

private:
    Model& model_;
};

} // namespace hashcode