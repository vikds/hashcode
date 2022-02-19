cmake_minimum_required(VERSION 2.8)

project (hashcode)

set(SOURCES
    src/main.cpp
)

add_executable(hashcode ${SOURCES})

target_include_directories(hashcode
    PRIVATE ${PROJECT_SOURCE_DIR}/src
)