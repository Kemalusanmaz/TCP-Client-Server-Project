# Minimum Cmake version
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(Server VERSION 1.0)

# CMake policy to fix Boost warning
cmake_policy(SET CMP0167 NEW)

# build with debug mode
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "-g") # add -g (debug symbol) in the c++ compiler)

# the location of Boost library that Cmake will find.
find_package(Boost 1.40.0 REQUIRED system)

# the path of header file
include_directories("C:/msys64/ucrt64/include" ${Boost_INCLUDE_DIRS})
link_directories("C:/msys64/ucrt64/lib")

# definition of source file
set(SOURCES src/server.cpp src/serverMain.cpp)

# define compile target
add_executable(Server ${SOURCES})

# Link Boost libraries to your target
target_include_directories(Server PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(Server ${Boost_LIBRARIES})
# Link ws2_32 library for Winsock functions
target_link_libraries(Server ws2_32)
