#pragma once

#include "fwd.hpp"

namespace hashcode
{

class Intersection {
public:
    Intersection(size_t id);

    size_t id() const {
      return id_;
    }

public:
    void Reset();

public:
    std::vector<Street*> incoming;
    std::vector<Street*> outgoing;
    size_t time_wasted;

private:
    size_t id_;
};

} // namespace hashcode