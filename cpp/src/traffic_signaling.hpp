#pragma once

#include "fwd.hpp"

#include "traffic_light.hpp"

namespace hashcode
{

class TrafficSignaling {
public:
    void SaveToFile(const std::string& file_name);

public:
    std::vector<TrafficLight> traffic_lights;
};

} // namespace hashcode