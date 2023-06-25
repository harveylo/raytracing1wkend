cmake_minimum_required(VERSION 3.22)

project(RTrender VERSION 0.1)

include_directories("include")

file(GLOB_RECURSE SRCS "src/*.cpp")
# file(GLOB_RECURSE UTILS "src/utils/*.cpp")
file(COPY ${CMAKE_SOURCE_DIR}/resources DESTINATION ${CMAKE_BINARY_DIR})

add_executable(render ${SRCS})
# add_library(UTIL ${UTILS})

set_property(TARGET render PROPERTY CXX_STANDARD 20)

# target_link_libraries(render UTIL UTIL)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wall -ferror-limit=0")
# set(CMAKE_VERBOSE_MAKEFILE ON)
