cmake_minimum_required(VERSION 3.22)

project(RTrender VERSION 0.1)

include_directories("include")

file(GLOB_RECURSE SRCS "src/*.cpp")

add_executable(render ${SRCS})

set_property(TARGET render PROPERTY CXX_STANDARD 20)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-non-virtual-dtor -Wall  -O3")
