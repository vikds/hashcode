#pragma once

#include "fwd.hpp"

namespace hashcode
{

class Solution {
public:
    Solution(Model& model);

    TrafficSignaling GetBestTrafficSignaling();

private:
    Model& model_;
};

} // namespace hashcode