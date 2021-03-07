#include "signaling.hpp"

#include <algorithm>
#include <fstream>
#include <limits>

#include "fwd.hpp"
#include "intersection.hpp"
#include "model.hpp"
#include "street.hpp"
#include "traffic_light.hpp"

namespace hashcode
{

const size_t Signaling::MAX_VALUE = std::numeric_limits<size_t>::max();

void Signaling::SaveToFile(const InputData& input_data, const std::string& file_name) {
    std::ofstream output(file_name);
    output << CountScheduledTrafficLights() << std::endl;
    for (const TrafficLight& traffic_light : traffic_lights) {
        if (traffic_light.scheduled_streets() == 0) {
            continue;
        }
        // output << traffic_light.intersection.get().id() << std::endl;
        // output << traffic_light.scheduled_streets() << std::endl;
        for (const ProceedSignal& proceed_signal : traffic_light.schedule) {
            if (proceed_signal.duration == 0) {
                continue;
            }
            // output << street_light.street->name() << " " << street_light.duration << std::endl;
        }
    }
}

size_t Signaling::GetNthWorstTrafficLightIndex(const Model& model, size_t nth) {
    if (nth >= traffic_lights.size()) {
        return MAX_VALUE;
    }
    std::nth_element(traffic_lights.begin(), traffic_lights.begin() + nth, traffic_lights.end(),
        [&] (const TrafficLight& lhs, const TrafficLight& rhs) {
            return model.intersections[lhs.intersection_id()].time_wasted > model.intersections[rhs.intersection_id()].time_wasted;
        }
    );
    if (model.intersections[traffic_lights[nth].intersection_id()].time_wasted == 0) {
        return MAX_VALUE;
    }
    return nth;
}

size_t Signaling::CountScheduledTrafficLights() const {
    size_t result = 0;
    for (const TrafficLight& traffic_light : traffic_lights) {
        if (traffic_light.scheduled_streets() > 0) {
            result++;
        }
    }
    return result;
}

} // namespace hashcode