#pragma once

#include "fwd.hpp"

namespace hashcode
{

class Car {
public:
    Car(size_t id, const std::vector<StreetRef>& path);

    size_t id() const {
        return id_;
    }
    size_t finish_time() const {
        return finish_time_;
    }
    size_t left_to_go() const {
        return left_to_go_;
    }
    const std::vector<StreetRef>& path() const {
        return path_;
    }
    bool has_finished() const {
        return position_ == path_.size();
    }

    void Reset();
    void Tick(size_t time);

private:
    void Turn(size_t time);

private:
    std::vector<StreetRef> path_;
    size_t id_;
    size_t position_ = 0;
    size_t finish_time_ = 0;
    size_t left_to_go_ = 0;
};

} // namespace hashcode