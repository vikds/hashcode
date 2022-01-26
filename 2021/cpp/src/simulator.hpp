#pragma once

#include "fwd.hpp"
#include "signaling.hpp"

namespace hashcode
{

class Simulator {
public:
    static void InitializeTrafficLights(Model& model, Signaling& signaling);
    static size_t Run(Model& model, Signaling& singnaling, const std::string& info);
};

} // namespace hashcode