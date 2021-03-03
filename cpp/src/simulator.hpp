#pragma once

#include "fwd.hpp"

namespace hashcode
{

class Simulator {
public:
    Simulator(Model& model);

    size_t Run(TrafficSignaling& traffic_signaling);

private:
    Model& model_;
};

} // namespace hashcode