#pragma once

#include "fwd.hpp"

namespace hashcode
{

struct Intersection {
public:
    Intersection(size_t id)
      : id(id)
    {}

    size_t id;
    std::vector<Street*> incoming;
    std::vector<Street*> outgoing;
};

} // namespace hashcode