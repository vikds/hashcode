#pragma once

#include "fwd.hpp"

#include <deque>

namespace hashcode
{

class Street {
public:
    Street(size_t id, std::string name, size_t travel_time);

    size_t id() const {
        return id_;
    }
    const std::string& name() const {
        return name_;
    }
    size_t travel_time() const {
        return travel_time_;
    }

public:
    void Reset();

public: // resetable
    std::deque<Car*> cars;
    size_t cars_expected;
    size_t time_wasted;
    bool is_green;

private:
    size_t id_;
    std::string name_;
    size_t travel_time_;
};

} // namespace hashcode