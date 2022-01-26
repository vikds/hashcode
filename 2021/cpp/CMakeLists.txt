cmake_minimum_required(VERSION 2.7)

project (hashcode)

set(SOURCES
    src/car.cpp
    src/intersection.cpp
    src/main.cpp
    src/model.cpp
    src/random_generator.cpp
    src/signaling.cpp
    src/simulator.cpp
    src/solution.cpp
    src/street.cpp
    src/timer.cpp
    src/traffic_light.cpp
)

add_executable(hashcode ${SOURCES})

target_include_directories(hashcode
    PRIVATE ${PROJECT_SOURCE_DIR}/src
)