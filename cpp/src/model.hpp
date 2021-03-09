#pragma once

#include "fwd.hpp"

#include "car.hpp"
#include "intersection.hpp"
#include "street.hpp"

namespace hashcode
{

struct Arguments {
    std::string input_file;
    std::string output_file;
    size_t rotations = 10;
    size_t attempts = 10;
};

struct InputData {
    InputData(const Arguments& arguments);

    Arguments arguments;
    size_t simulation_time;
    size_t intersections_num;
    size_t streets_num;
    size_t cars_num;
    size_t finish_bonus;

    struct Street {
        size_t intersection_id;
        std::string name;
        size_t travel_time;
    };
    std::vector<Street> streets;

    struct Car {
        std::vector<size_t> path;
    };
    std::vector<Car> cars;
};

class Model {
public:
    Model(const InputData& data);

    size_t simulation_time() const {
        return input_data_.simulation_time;
    }
    size_t finish_bonus() const {
        return input_data_.finish_bonus;
    }

    void Reset();
    void CountCarsExpectedOnTheStreets();

private:
    void InitCarsOnStreets();

public:
    std::vector<Car> cars;
    std::vector<Intersection> intersections;
    std::vector<Street> streets;

private:
    const InputData& input_data_;
};

} // namespace hashcode