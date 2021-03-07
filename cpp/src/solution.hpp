#pragma once

#include "fwd.hpp"

#include "signaling.hpp"

namespace hashcode
{

class Solution {
public:
    Solution(const InputData& input_data, size_t attempts);

    Signaling GetBestSignaling();

private:
    TrafficLight* GetNthWorstTrafficLight(TrafficLights& traffic_lights, size_t index);
    bool ExtendMaxTimeWastedStreet(Model& model, TrafficLight& traffic_light);

private:
    const InputData& input_data_;
    size_t attempts_;
};

} // namespace hashcode