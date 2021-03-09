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

Solution::Solution(const InputData& input_data)
  : input_data_(input_data)
{}

Signaling Solution::GetBestSignaling() {
    Model model(input_data_); // threads? initialize models[threads]
    size_t score = 0;
    size_t best_score = 0;
    size_t iterations = 0;
    size_t update_iteration = 0;
    Signaling best_signaling;
    Timer solution_timer("Get the best result timing");
    std::cout << "Starting with signaling score: " << best_score << std::endl;
    Simulator::InitializeTrafficLights(model, best_signaling); // run on treads[0]?
    size_t attempts = input_data_.arguments.attempts;
    size_t rotations = input_data_.arguments.rotations;
    for (size_t attempt = 0; attempt < attempts;) { // attempt = nth worst element
        std::cout << DIVIDING_LINE << std::endl;
        std::cout << "CURRENT SCORE " << best_score << " (attempt: " << attempt << ")" << std::endl << std::flush;
        std::cout << DIVIDING_LINE << std::endl; 
        score = Simulator::Run(model, best_signaling, std::to_string(attempt));
        size_t index = best_signaling.GetNthWorstTrafficLightIndex(model, attempt);
        iterations++;
        if (score > best_score) {
            best_score = score;
            attempt = 0;
            std::cout << "Better score: " << score << " (" << ++update_iteration << ")" << std::endl;
            if (update_iteration >= 10) {
                best_signaling.SaveToFile(input_data_);
                update_iteration = 0;
            }
        }
        if (index >= best_signaling.traffic_lights.size()) {
            std::cout << DIVIDING_LINE << std::endl; 
            std::cout << "FINAL SCORE: " << best_score << std::endl;
            std::cout << DIVIDING_LINE << std::endl;
            break;
        }
        bool score_updated = false;
        // rotation can be paralleled for different model[thread]: OMP?
        size_t schedule_size = best_signaling.traffic_lights[index].schedule.size();
        for (size_t rotation = 1; rotation < std::min(rotations, schedule_size); rotation++, iterations++) {
            Signaling signaling = best_signaling;
            Schedule& worst_schedule = signaling.traffic_lights[index].schedule;
            std::rotate(worst_schedule.begin(), worst_schedule.begin() + rotation, worst_schedule.end());
            score = Simulator::Run(model, signaling, std::string("Schedule rotation ") + std::to_string(rotation));
            if (score > best_score) {
                best_signaling = signaling;
                score_updated = true;
                best_score = score;
                std::cout << "Better score: " << score << " (" << ++update_iteration << ")" << std::endl;
                if (update_iteration >= 10) {
                    best_signaling.SaveToFile(input_data_);
                    update_iteration = 0;
                }
            }
        }
        // run main thread[0]
        if (score_updated) {
            attempt = 0;
            continue;
        }
        TrafficLight& worst_traffic_light = best_signaling.traffic_lights[index];
        size_t street_id = worst_traffic_light.IncrWorstStreetDuration(model);
        if (street_id < MAX_VALUE) {
            std::cout << "Increased duration for street: " << input_data_.streets[street_id].name << std::endl;
        }
        attempt++;
    }
    std::cout << DIVIDING_LINE << std::endl;
    std::cout << "BEST SCORE got in " << iterations << " iterations: " <<  best_score << std::endl;
    std::cout << DIVIDING_LINE << std::endl;
    return best_signaling;
}

} // namespace hashcode