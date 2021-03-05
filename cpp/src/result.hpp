#pragma once

#include "fwd.hpp"

#include "traffic_light.hpp"

namespace hashcode
{

class Result {
public:
    void SaveToFile(const std::string& file_name);

private:
    size_t CountScheduledTrafficLights() const;

public:
    std::vector<TrafficLight> traffic_lights;
};

} // namespace hashcode