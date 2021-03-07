#pragma once

#include "fwd.hpp"

#include "traffic_light.hpp"

namespace hashcode
{

class Signaling {
    static const size_t MAX_VALUE;

public:
    void SaveToFile(const InputData& input_data, const std::string& file_name);
    size_t GetNthWorstTrafficLightIndex(const Model& model, size_t nth);

private:
    size_t CountScheduledTrafficLights() const;

public:
    TrafficLights traffic_lights;
};

} // namespace hashcode