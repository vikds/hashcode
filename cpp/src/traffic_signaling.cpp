#include "traffic_signaling.hpp"

#include <fstream>

#include "fwd.hpp"
#include "intersection.hpp"
#include "street.hpp"
#include "traffic_light.hpp"

namespace hashcode
{

size_t TrafficSignaling::scheduled_traffic_lights() const {
    size_t result = 0;
    for (std::vector<TrafficLight>::const_iterator traffic_light = traffic_lights.begin(); traffic_light != traffic_lights.end(); traffic_light++) {
        if (traffic_light->scheduled_streets() > 0) {
            result++;
        }
    }
    return result;
}

void TrafficSignaling::SaveToFile(const std::string& file_name) {
    std::ofstream output(file_name);
    output << scheduled_traffic_lights() << std::endl;
    for (std::vector<TrafficLight>::iterator traffic_light = traffic_lights.begin(); traffic_light != traffic_lights.end(); traffic_light++) {
        if (traffic_light->scheduled_streets() == 0) {
            continue;
        }
        output << traffic_light->intersection().id() << std::endl;
        output << traffic_light->scheduled_streets() << std::endl;
        for (Schedule::const_iterator street_light = traffic_light->schedule().begin(); street_light != traffic_light->schedule().end(); street_light++) {
            if (street_light->duration == 0) {
                continue;
            }
            output << street_light->street->name() << " " << street_light->duration << std::endl;
        }
    }
}

void TrafficSignaling::Reset() {
    for (std::vector<TrafficLight>::iterator traffic_light = traffic_lights.begin(); traffic_light != traffic_lights.end(); traffic_light++) {
        traffic_light->Reset();
    }
}

} // namespace hashcode