#include "random_generator.hpp"

namespace hashcode
{

RandomGenerator::RandomGenerator(size_t min, size_t max, size_t seed)
  : generator_(seed),
    int_distribution_(min, max),
    min_(min),
    max_(max),
    seed_(seed)
{}

size_t RandomGenerator::GetNextInt() {
    return int_distribution_(generator_);
}

double RandomGenerator::GetNextReal() {
    return static_cast< double >(GetNextInt()) / max();
}

} // namespace hashcode


