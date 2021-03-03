cmake_minimum_required(VERSION 3.18)

project (hashcode)

set(SOURCES
    src/car.cpp
    src/intersection.cpp
    src/main.cpp
    src/model.cpp
    src/simulator.cpp
    src/solution.cpp
    src/street.cpp
    src/traffic_light.cpp
    src/traffic_signaling.cpp
)

add_executable(hashcode ${SOURCES})

target_include_directories(hashcode
    PRIVATE ${PROJECT_SOURCE_DIR}/src
)