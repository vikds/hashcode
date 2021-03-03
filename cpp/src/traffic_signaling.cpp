#include "traffic_signaling.hpp"

#include <fstream>

#include "fwd.hpp"
#include "intersection.hpp"
#include "street.hpp"
#include "traffic_light.hpp"

namespace hashcode
{

void TrafficSignaling::SaveToFile(const std::string& file_name) {
    std::ofstream output(file_name);
    output << traffic_lights.size() << std::endl;
    for (std::vector<TrafficLight>::iterator traffic_light = traffic_lights.begin(); traffic_light != traffic_lights.end(); traffic_light++) {
        output << traffic_light->intersection().id() << std::endl;
        output << traffic_light->schedule().size() << std::endl;
        for (Schedule::const_iterator street_light = traffic_light->schedule().begin(); street_light != traffic_light->schedule().end(); street_light++) {
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