#include "car.hpp"

#include "fwd.hpp"
#include "street.hpp"

namespace hashcode
{

Car::Car(size_t id, const std::vector<StreetRef>& path)
  : id_(id),
    path_(path),
    position_(0),
    finish_time_(0),
    left_to_go_(0)
{}

void Car::Reset() {
    position_ = 0;
    finish_time_ = 0;
    left_to_go_ = 0;
}

void Car::Tick(size_t time) {
    if (has_finished()) {
        return;
    }
    if (left_to_go_ > 1) {
        left_to_go_--;
        return;
    }
    Street& street = path_[position_];
    if (street.IsAllowedToTurn(*this)) {
        Turn(time);
    } else {
        street.AddTimeWasted();
    }
}

void Car::Turn(size_t time) {
    Street& curr_street = path_[position_];
    curr_street.cars.pop_front();
    curr_street.car_passed = true;
    curr_street.cars_passed++;
    position_++;
    if (has_finished()) {
        finish_time_ = time;
        return;
    }
    Street& next_street = path_[position_];
    next_street.cars.push_back(*this);
    left_to_go_ = next_street.travel_time();
}


} // namespace hashcode