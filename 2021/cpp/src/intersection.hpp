#pragma once

#include "fwd.hpp"

namespace hashcode
{

class Intersection {
public:
    explicit Intersection(size_t id);

    size_t id() const {
        return id_;
    }

    void Reset();

public:
    std::vector<StreetRef> streets;
    size_t time_wasted = 0;

private:
    size_t id_;
};

} // namespace hashcode