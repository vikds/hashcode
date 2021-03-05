#include "intersection.hpp"

#include "fwd.hpp"
#include "street.hpp"

namespace hashcode
{

Intersection::Intersection(size_t id)
  : id_(id),
    time_wasted(0)  
{}

void Intersection::Reset() {
    time_wasted = 0;
}

} // namespace hashcode