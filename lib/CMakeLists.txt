cmake_minimum_required(VERSION 3.5)
project(huffmanlib)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -pedantic -g")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address,undefined -D_GLIBCXX_DEBUG")
set(CMAKE_CXX_FLAGS_RELEASE  "${CMAKE_CXX_FLAGS} -O3")

add_library(huffmanlib
        decoder.cpp
        decoder.h
        encoder.cpp
        encoder.h tree.cpp tree.h counter.cpp counter.h bit_vector.cpp bit_vector.h)