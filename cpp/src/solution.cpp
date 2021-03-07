#include "solution.hpp"

#include <algorithm>

#include "car.hpp"
#include "fwd.hpp"
#include "model.hpp"
#include "simulator.hpp"
#include "street.hpp"
#include "timer.hpp"

namespace hashcode
{

static const std::string DIVIDING_LINE = "----------------------------------------------------------";

Solution::Solution(const InputData& input_data, size_t attempts)
  : input_data_(input_data),
    attempts_(attempts)
{}

Signaling Solution::GetBestSignaling() {
    Model model(input_data_); // threads? initialize models[threads]
    size_t score = 0;
    size_t best_score = 0;
    Signaling best_signaling;
    Timer solution_timer("Get the best result");
    std::cout << "Starting with signaling score: " << best_score << std::endl;
    Simulator::InitializeTrafficLights(model, best_signaling); // run on treads[0]?
    for (size_t attempt = 0; attempt < attempts_; attempt++) { // attempt = nth
        std::cout << DIVIDING_LINE << std::endl;
        std::cout << "CURRENT SCORE " << best_score << " (attempt: " << attempt << ")" << std::endl;
        std::cout << DIVIDING_LINE << std::endl; 
        score = Simulator::Run(model, best_signaling);
        size_t index = best_signaling.GetNthWorstTrafficLightIndex(model, attempt);
        if (score > best_score ) {
            best_score = score;
            attempt = 0;
        }
        if (index >= best_signaling.traffic_lights.size()) {
            std::cout << DIVIDING_LINE << std::endl; 
            std::cout << "FINAL SCORE: " << best_score << std::endl;
            std::cout << DIVIDING_LINE << std::endl; 
            return best_signaling;
        }
        bool score_updated = false;
        size_t schedule_size = best_signaling.traffic_lights[index].schedule.size();
        // rotation can be paralleled for different model[thread]: OMP?
        for (size_t rotation = 1; rotation < schedule_size; rotation++) {
            Timer rotation_timer("Rotation of schedule " + std::to_string(rotation));
            Signaling signaling = best_signaling;
            Schedule& worst_schedule = signaling.traffic_lights[index].schedule;
            std::rotate(worst_schedule.begin(), worst_schedule.begin() + rotation, worst_schedule.end());
            score = Simulator::Run(model, signaling);
            if (score > best_score) {
                best_signaling = signaling;
                score_updated = true;
                best_score = score;
            }
        }
        // run main thread[0]
        if (score_updated) {
            attempt = 0;
            continue;
        }
        model.Reset();
        TrafficLight& worst_traffic_light = best_signaling.traffic_lights[index];
        if (worst_traffic_light.IncrWorstStreetDuration(model)) {
            attempt = 0;
        }
    }
    std::cout << DIVIDING_LINE << std::endl;
    std::cout << "BEST SCORE: " << best_score << std::endl;
    std::cout << DIVIDING_LINE << std::endl;
    return best_signaling;
}

} // namespace hashcode