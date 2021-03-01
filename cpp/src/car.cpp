#include "car.hpp"

#include "street.hpp"

#include "fwd.hpp"

namespace hashcode
{

Car::Car(const std::vector<Street*>& path)
  : path_(path),
    position_(0),
    finish_time_(0),
    left_to_go_(0)
{
    min_required_time_ = 1; // finish turn
    for (size_t i = 1; i < path_.size(); i++) {
        Street* street = path_[i];
        min_required_time_ += 1; // make a turn
        min_required_time_ += street->travel_time();
    }
}

Street* Car::current_street() {
    return path_[position_];
}

void Car::Tick(size_t time) {
    if (HasFinished()) {
        return;
    }
    if (IsWaiting()) {
        Street* curr_street = current_street();
        if (curr_street->is_green && this == curr_street->cars.front()) {
            Turn(time);
        }
    } else {
        left_to_go_--;
    }
}

void Car::Reset() {
    position_ = 0;
    finish_time_ = 0;
    left_to_go_ = 0;
}

bool Car::HasFinished() const {
    return position_ == path_.size();
}

bool Car::IsWaiting() const {
    return left_to_go_ == 0;
}

void Car::Turn(size_t time) {
    Street* curr_street = current_street();
    curr_street->cars.pop_front();
    position_++;
    if (HasFinished()) {
        finish_time_ = time;
        return;
    }
    Street* next_street = path_[position_];
    next_street->cars.push_back(this);
    left_to_go_ = next_street->travel_time();
}


} // namespace hashcode