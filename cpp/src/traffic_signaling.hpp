#pragma once

#include "fwd.hpp"

#include "traffic_light.hpp"

namespace hashcode
{

class TrafficSignaling {
public:
    size_t scheduled_traffic_lights() const;

public:
    void SaveToFile(const std::string& file_name);

    void Reset();

public:
    std::vector<TrafficLight> traffic_lights;
};

} // namespace hashcode