cmake_minimum_required(VERSION 3.5)
project(huffman_tests)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -pedantic -g")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address,undefined -D_GLIBCXX_DEBUG")
set(CMAKE_CXX_FLAGS_RELEASE  "${CMAKE_CXX_FLAGS} -O3")

include_directories(${CMAKE_SOURCE_DIR}/lib)

add_executable(lib_tests lib_tests.cpp
                         gtest/gtest-all.cc
                         gtest/gtest.h
                         gtest/gtest_main.cc)
target_link_libraries(lib_tests huffmanlib -lpthread)