cmake_minimum_required(VERSION 3.7)
project(eternaldom)

set(CMAKE_CXX_STANDARD 14)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-O3")

#find_package(PythonLibs 3.5 REQUIRED)
#include_directories( ${PYTHON_INCLUDE_DIRS} )

#find_package(Boost COMPONENTS python3 REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

include_directories(include)

set(SOURCE_FILES  src/Graph.cpp src/Arena.cpp src/Util.cpp src/Rules.cpp src/Orientation.cpp src/Tournament.cpp src/main.cpp)

add_executable(eds ${SOURCE_FILES})