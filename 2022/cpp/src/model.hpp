#pragma once

#include "fwd.hpp"

namespace hashcode
{

struct Arguments {
    std::string input_file;
    std::string output_file;
    size_t seed = 0;
};

} // namespace hashcode