#include "traffic_signaling.hpp"

#include <fstream>

#include "fwd.hpp"
#include "street.hpp"
#include "traffic_light.hpp"

namespace hashcode
{

void TrafficSignaling::SaveToFile(const std::string& file_name) {
    std::ofstream output(file_name);
    output << traffic_lights.size() << std::endl;
    for (std::vector<TrafficLight>::const_iterator light = traffic_lights.begin(); light != traffic_lights.end(); light++) {
        const TrafficLight& traffic_light = *light;
        output << traffic_light.intersection_id() << std::endl;
        output << traffic_light.schedule().size() << std::endl;
        for (Schedule::const_iterator green_light = traffic_light.schedule().begin(); green_light != traffic_light.schedule().end(); green_light++) {
            output << green_light->street->name() << " " << green_light->duration << std::endl;
        }
    }
}

void TrafficSignaling::Reset() {
    for (std::vector<TrafficLight>::iterator light = traffic_lights.begin(); light != traffic_lights.end(); light++) {
        light->Reset();
    }
}

} // namespace hashcode