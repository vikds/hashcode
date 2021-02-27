cmake_minimum_required(VERSION 3.18)

project (hashcode)

set(SOURCES
    src/hello.cpp
    src/main.cpp
)

add_executable(hashcode ${SOURCES})

target_include_directories(hashcode
    PRIVATE ${PROJECT_SOURCE_DIR}/src
)