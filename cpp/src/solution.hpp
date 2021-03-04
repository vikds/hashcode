#pragma once

#include "fwd.hpp"

namespace hashcode
{

class Solution {
public:
    Solution(Model& model, size_t attempts);

    TrafficSignaling GetBestTrafficSignaling();

private:
    TrafficLight* GetMaxCarsJammedTrafficLight(std::vector<TrafficLight>& traffic_lights);
    TrafficLight* GetAvgCarsJammedTrafficLight(std::vector<TrafficLight>& traffic_lights);
    bool OpenAvgCarsJammedStreet(TrafficLight& traffic_light);
    bool OpenMaxCarsJammedStreet(TrafficLight& traffic_light);
    void CountCarsExpectedOnTheStreets();
    TrafficSignaling InitializeWithCarsExpected();
    
private:
    Model& model_;
    size_t attempts_;
};

} // namespace hashcode