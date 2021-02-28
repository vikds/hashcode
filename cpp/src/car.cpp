#include "car.hpp"

#include "street.hpp"

#include "fwd.hpp"

namespace hashcode
{

Car::Car(const std::vector<Street*>& path)
  : path_(path),
    position_(0),
    finish_time_(0),
    left_to_go_(0) {
    for (size_t i = 1; i < path_.size(); i++) {
        Street* street = path_[i];
        min_required_time_ += 1; // make a Turn();
        min_required_time_ += street->travel_time();
    }
}

Street* Car::current_street() {
    return path_[position_];
}

void Car::Tick() {
    return; // TODO
}

void Car::Reset() {
    position_ = 0;
    finish_time_ = 0;
    left_to_go_ = 0;
}

bool Car::HasFinished() const {
    return position_ == path_.size() - 1 && left_to_go_ == 0;
}

bool Car::IsWaiting() const {
    return left_to_go_ == 0;
}

void Car::Turn() {
    size_t next = position_ + 1;
    if (next >= path_.size()) {
        return;
    }
    Street* next_street = path_[next];
}


} // namespace hashcode