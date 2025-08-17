cmake_minimum_required(VERSION 4.0)
project(provisioner)

set(CMAKE_CXX_STANDARD 26)

file(GLOB_RECURSE SRC_FILES
        source/*.cpp
        source/*.c
        source/*.hpp
        source/*.h,
)

find_package(CLI11 CONFIG REQUIRED)

add_executable(provisioner ${SRC_FILES})

target_link_libraries(provisioner PRIVATE CLI11::CLI11)