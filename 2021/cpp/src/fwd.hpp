#pragma once

#include <functional>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

// forward declarations
namespace hashcode
{

class Car;
struct InputData;
class Intersection;
class Model;
class Result;
class Simulator;
class Solution;
class Street;
struct ProceedSignal;
class TrafficLight;

// schedule of a particular TrafficLight
using Schedule = std::vector<ProceedSignal>;

// get rid of raw poiners
using CarRef = std::reference_wrapper<Car>;
using StreetRef = std::reference_wrapper<Street>;
using IntersectionRef = std::reference_wrapper<Intersection>;

// for convenience
using TrafficLights = std::vector<TrafficLight>;

// for edge cases
static const size_t MAX_VALUE = std::numeric_limits<size_t>::max();

} // namespace hashcode