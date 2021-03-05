#pragma once

#include "fwd.hpp"

namespace hashcode
{

class Solution {
public:
    Solution(Model& model, size_t attempts);

    Result GetBestResult();

private:
    TrafficLight* GetMaxTimeWastedTrafficLight(std::vector<TrafficLight>& traffic_lights, size_t index);
    bool ExtendMaxTimeWastedStreet(TrafficLight& traffic_light);

private:
    Model& model_;
    size_t attempts_;
};

} // namespace hashcode