#include "result.hpp"

#include <fstream>

#include "fwd.hpp"
#include "intersection.hpp"
#include "street.hpp"
#include "traffic_light.hpp"

namespace hashcode
{

void Result::SaveToFile(const std::string& file_name) {
    std::ofstream output(file_name);
    output << CountScheduledTrafficLights() << std::endl;
    for (const TrafficLight& traffic_light : traffic_lights) {
        if (traffic_light.scheduled_streets() == 0) {
            continue;
        }
        output << traffic_light.intersection->id() << std::endl;
        output << traffic_light.scheduled_streets() << std::endl;
        for (const StreetLight& street_light : traffic_light.schedule) {
            if (street_light.duration == 0) {
                continue;
            }
            // output << street_light.street->name() << " " << street_light.duration << std::endl;
        }
    }
}

size_t Result::CountScheduledTrafficLights() const {
    size_t result = 0;
    for (const TrafficLight& traffic_light : traffic_lights) {
        if (traffic_light.scheduled_streets() > 0) {
            result++;
        }
    }
    return result;
}

} // namespace hashcode