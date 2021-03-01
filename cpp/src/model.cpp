#include "model.hpp"

#include <fstream>
#include <unordered_map>

#include "fwd.hpp"
#include "intersection.hpp"

namespace hashcode
{

void Model::LoadFromFile(const std::string& file_name) {
    std::ifstream input(file_name);
    input >> simulation_time_;
    size_t intersections;
    input >> intersections;
    size_t streets;
    input >> streets;
    size_t cars;
    input >> cars;
    input >> finish_bonus_;

    for (size_t id = 0; id < intersections; id++) {
        intersections_.push_back(Intersection(id));
    }

    size_t beg_is_id;
    size_t end_is_id;
    std::string name;
    size_t travel_time;
    std::unordered_map<std::string, size_t> street_name_2_id;
    for (size_t id = 0; id < streets; id++) {
        input >> beg_is_id;
        input >> end_is_id;
        input >> name;
        input >> travel_time;

        streets_.push_back(Street(id, name, travel_time));
        intersections_[beg_is_id].outgoing.push_back(&streets_.back());
        intersections_[end_is_id].incoming.push_back(&streets_.back());
        street_name_2_id[name] = id;
    }

    size_t locations;
    std::vector<Street*> path;
    for (size_t i = 0; i < cars; i++) {
        path.clear();
        input >> locations;
        for (size_t j = 0; j < locations; j++) {
            input >> name;
            size_t street_id = street_name_2_id[name];
            path.push_back(&streets_[street_id]);
        }
        cars_.push_back(Car(path));
    }

    PushCarsToStreets();
}

void Model::Reset() {
    for (std::vector<Car>::iterator car = cars().begin(); car != cars().end(); car++) {
        car->Reset();
    }
    PushCarsToStreets();
}

void Model::PushCarsToStreets() {
    for (std::vector<Street>::iterator street = streets().begin(); street != streets().end(); street++) {
        street->Reset();
    }
    for (std::vector<Car>::iterator car = cars().begin(); car != cars().end(); car++) {
        if (car->path().empty()) {
            continue;
        }
        Street* first_street = car->path().front();
        first_street->cars.push_back(&*car);
    }
}

} // namespace hashcode