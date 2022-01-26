#include "intersection.hpp"

#include "fwd.hpp"
#include "street.hpp"

namespace hashcode
{

Intersection::Intersection(size_t id)
  : id_(id)
{}

void Intersection::Reset() {
    time_wasted = 0;
}

} // namespace hashcode