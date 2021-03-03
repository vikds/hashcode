#include "car.hpp"

#include "fwd.hpp"
#include "street.hpp"

namespace hashcode
{

Car::Car(const std::vector<Street*>& path)
  : path_(path),
    position_(0),
    finish_time_(0),
    left_to_go_(0)
{
    min_required_time_ = 0;
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
    if (!IsWaiting()) {
        return;
    }
    Street* street = path_[position_];
    if (street->IsAllowedToTurn(this)) {
        Turn(time);
    } else {
        street->TickCosts();
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
    if (this != curr_street->cars.front()) {
        std::ostringstream oss;
        oss << "A moving car is not the first in the queue on the street: " << curr_street->name();
        throw std::runtime_error(oss.str());
    }
    curr_street->cars.pop_front();
    curr_street->car_passed = true;
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