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
    min_required_time_ = 1;
    for (std::vector<Street*>::iterator it = path_.begin(); it != path_.end(); it++) {
        Street* street = *it;
        min_required_time_ += street->travel_time();
    }
}

void Car::Tick(size_t time) {
    if (HasFinished()) {
        return;
    }
    if (left_to_go_ > 0) {
        left_to_go_--;
    }
    if (IsWaiting()) {
        Street* street = path_[position_];
        if (street->is_green && !street->ticked && this == street->cars.front()) {
            Turn(time);
        } else {
            street->time_wasted++;
        }
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
    Street* curr_street = path_[position_];
    curr_street->cars.pop_front();
    curr_street->ticked = true;
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