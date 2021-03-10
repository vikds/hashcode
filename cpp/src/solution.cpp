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
    Signaling best_signaling;
    Timer solution_timer("Get the best result timing");
    std::cout << "Starting with signaling score: " << best_score << std::endl;
    Simulator::InitializeTrafficLights(model, best_signaling); // run on treads[0]?
    size_t attempts = input_data_.arguments.attempts;
    size_t rotations = input_data_.arguments.rotations;
    for (size_t attempt = 0; attempt < attempts; iterations++) { // attempt = nth worst element
        std::cout << DIVIDING_LINE << std::endl;
        std::cout << "CURRENT SCORE " << best_score << " (attempt: " << attempt << ")" << std::endl << std::flush;
        std::cout << DIVIDING_LINE << std::endl; 
        score = Simulator::Run(model, best_signaling, std::to_string(attempt));
        size_t index = best_signaling.GetNthWorstTrafficLightIndex(model, attempt);
        if (score > best_score) {
            best_score = score;
            attempt = 0;
            std::cout << "Better score: " << best_score << std::endl;
            best_signaling.SaveToFile(input_data_, best_score);
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
        size_t max_rotations = model.seed() ? rotations : std::min(rotations, schedule_size);
        for (size_t rotation = 1; rotation < max_rotations; rotation++, iterations++) {
            Signaling signaling = best_signaling;
            Schedule& worst_schedule = signaling.traffic_lights[index].schedule;
            if (model.seed()) {
                std::shuffle(worst_schedule.begin(), worst_schedule.end(), model.random_generator);
            } else {
                std::rotate(worst_schedule.begin(), worst_schedule.begin() + rotation, worst_schedule.end());
            }
            score = Simulator::Run(model, signaling, std::string("Schedule rotation ") + std::to_string(rotation));
            if (score > best_score) {
                best_signaling = signaling;
                score_updated = true;
                best_score = score;
                std::cout << "Better score: " << best_score << std::endl;
                best_signaling.SaveToFile(input_data_, best_score);
            }
        }
        // run main thread[0]
        if (score_updated) {
            attempt = 0;
            continue;
        }
        TrafficLight& worst_traffic_light = best_signaling.traffic_lights[index];
        std::vector<size_t> decreased_streets = worst_traffic_light.DecrFreeStreetsDuration(model);
        if (!decreased_streets.empty()) {
            std::cout << "Decreased duration for streets: ";
            for (size_t i = 0; i < decreased_streets.size(); i++) {
                size_t street_id = decreased_streets[i];
                if (i) {
                    std::cout << ", ";
                }
                std::cout << input_data_.streets[street_id].name;
            }
            std::cout << std::endl;
            attempt++;
            continue;
        }
        double random = model.random_generator.GetNextReal();
        size_t street_id = random < 0.80 ? worst_traffic_light.IncrWorstStreetDuration(model) : worst_traffic_light.DecrFreeStreetDuration(model);
        if (street_id < MAX_VALUE) {
            std::cout << "Changed duration for street: " << input_data_.streets[street_id].name << std::endl;
            attempt++;
            continue;
        }
        attempt++;
    }
    std::cout << DIVIDING_LINE << std::endl;
    std::cout << "BEST SCORE got in " << iterations << " iterations: " <<  best_score << std::endl;
    std::cout << DIVIDING_LINE << std::endl;
    return best_signaling;
}

} // namespace hashcode