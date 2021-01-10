cmake_minimum_required(VERSION 3.0)
project(pubsupp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-fpermissive")

add_library(pubsupp SHARED pubsupp.h)
add_library(pubsuppstatic STATIC pubsupp.h)
set_target_properties(pubsupp PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(pubsuppstatic PROPERTIES LINKER_LANGUAGE CXX)

add_executable(test_pubsub pubsupp.h tests/pubsub.cpp)
add_executable(test_threads pubsupp.h tests/threads.cpp)
target_link_libraries(test_threads pthread)