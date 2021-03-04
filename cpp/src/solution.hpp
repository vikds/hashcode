#pragma once

#include "fwd.hpp"

namespace hashcode
{

class Solution {
public:
    Solution(Model& model, size_t attempts);

    TrafficSignaling GetBestTrafficSignaling();

private:
    bool ChangeTrafficLightSchedule(TrafficLight& traffic_light);
    void CountCarsExpectedOnTheStreets();
    TrafficSignaling InitializeWithCarsExpected();
    
private:
    Model& model_;
    size_t attempts_;
};

} // namespace hashcode