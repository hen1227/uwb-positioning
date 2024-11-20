cmake_minimum_required(VERSION 3.29)
project(UWBPositioning)

set(CMAKE_CXX_STANDARD 20)

add_library(UWBPositioning STATIC
        src/UWBPositioning.h
        "src/UWBAnchor.cpp"
        src/UWBAnchor.h
        src/UWBTag.h
        src/UWBTag.cpp)
