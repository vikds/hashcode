#pragma once

#include "fwd.hpp"

#include "car.hpp"
#include "intersection.hpp"
#include "street.hpp"

namespace hashcode
{

class Model {
public:
    void LoadFromFile(const std::string& file_name);

    size_t simulation_time() const {
        return simulation_time_;
    }
    size_t finish_bonus() const {
        return finish_bonus_;
    }
    std::vector<Car>& cars() {
        return cars_;
    }
    std::vector<Intersection>& intersections() {
        return intersections_;
    }
    std::vector<Street>& streets() {
        return streets_;
    }

public:
    void Reset();

private:
    void PushCarsToStreets();

private:
    size_t simulation_time_; // up to 10k
    size_t finish_bonus_; // up to 1k

    std::vector<Car> cars_;
    std::vector<Intersection> intersections_;
    std::vector<Street> streets_;
};

} // namespace hashcode