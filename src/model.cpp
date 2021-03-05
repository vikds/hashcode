#include "model.hpp"

#include <fstream>
#include <iterator>
#include <unordered_map>

#include "fwd.hpp"
#include "intersection.hpp"

namespace hashcode
{

ModelData::ModelData(const std::string& file_name) {
    std::ifstream input(file_name);
    input >> simulation_time;
    input >> intersections_num;
    input >> streets_num;
    input >> cars_num;
    input >> finish_bonus;

    std::cout << "Simulation time: " << simulation_time << std::endl;
    std::cout << "Intersections count: " << intersections_num << std::endl;
    std::cout << "Streets count: " << streets_num << std::endl;
    std::cout << "Cars count: " << cars_num << std::endl;
    std::cout << "Finish bonus: " << finish_bonus << std::endl;

    std::string name;
    size_t travel_time;
    size_t intersection_id;
    std::unordered_map<std::string, size_t> street_name_2_id;
    for (size_t id = 0; id < streets_num; id++) {
        input >> intersection_id; // begin
        input >> intersection_id; // end
        input >> name;
        input >> travel_time;

        streets.push_back(Street{intersection_id, name, travel_time});
        street_name_2_id[name] = id;
    }

    size_t length;
    std::vector<size_t> path;
    for (size_t id = 0; id < cars_num; id++) {
        path.clear();
        input >> length;
        for (size_t l = 0; l < length; l++) {
            input >> name;
            path.push_back(street_name_2_id[name]);
        }
        cars.push_back(Car{path});
    }
}

Model::Model(const ModelData& data)
  : data_(data)
{
    intersections.reserve(data_.intersections_num);
    for (size_t id = 0; id < data_.intersections_num; id++) {
        intersections.push_back(Intersection(id));
    }

    streets.reserve(data_.streets_num);
    for (const ModelData::Street& street : data_.streets) {
        Intersection& intersection = intersections[street.intersection_id];
        streets.push_back(Street(intersection, street.name, street.travel_time));
        intersection.streets.push_back(&streets.back());
    }

    size_t locations;
    std::vector<Street*> path;
    cars.reserve(data_.cars_num);
    for (const ModelData::Car& car : data_.cars) {
        for (const size_t& street_id : car.path) {
            path.push_back(&streets[street_id]);
        }
        cars.push_back(Car(path));
    }
    InitCarsOnStreets();
}

void Model::Reset() {
    for (Intersection& intersection : intersections) {
        intersection.Reset();
    }
    InitCarsOnStreets();
}

void Model::InitCarsOnStreets() {
    for (Street& street : streets) {
        street.Reset();
    }
    for (Car& car : cars) {
        car.Reset();
        if (car.path().empty()) {
            continue;
        }
        Street* street = car.path().front();
        street->cars.push_back(&car);
    }
}

void Model::CountCarsExpectedOnTheStreets() {
    InitCarsOnStreets();

    for (Car& car : cars) {
        size_t travel_time = 0;
        for (auto it = car.path().begin(); it != car.path().end(); it++) {
            if (travel_time > simulation_time()) {
                break;
            }
            Street* street = *it;
            if (it != car.path().begin()) {
                travel_time += street->travel_time();
            }
            street->cars_expected++;
        }
    }
}

} // namespace hashcode