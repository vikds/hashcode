#pragma once

#include <random>


namespace hashcode
{

class RandomGenerator
{
    using GeneratorType = std::mt19937;
    using UniformIntDistributionType = std::uniform_int_distribution<>;

public:
    typedef size_t result_type; // for std::shuffle(..)

    RandomGenerator(size_t min, size_t max, size_t seed);

    size_t GetNextInt();
    double GetNextReal();

    size_t min() const { return min_; }
    size_t max() const { return max_; }
    size_t seed() const { return seed_; }

    size_t operator()() { return GetNextInt(); }

private:
    GeneratorType generator_;
    UniformIntDistributionType int_distribution_;
    size_t min_;
    size_t max_;
    size_t seed_;
};

} // namespace hashcode
