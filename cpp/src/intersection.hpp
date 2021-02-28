#pragma once

#include "fwd.hpp"

namespace hashcode
{

struct Intersection {
    std::vector<Street*> incoming;
    std::vector<Street*> outgoing;
};

} // namespace hashcode