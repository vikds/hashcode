#include "street.hpp"

#include "fwd.hpp"

namespace hashcode
{

void Street::Reset() {
    cars.clear();
    cars_expected = 0;
    time_wasted = 0;
}

} // namespace hashcode