# Minimum Cmake version
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(Client VERSION 1.0)

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

# the path of header file
include_directories(include)

# definition of source file
set(SOURCES
    src/qemuM1553Setup.cpp src/messageHandler.cpp src/qemuClientObject.cpp
    src/clientMain.cpp)

# define compile target
add_executable(Client ${SOURCES})

# Link Boost libraries to your target
target_include_directories(Client PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(Client ${Boost_LIBRARIES})

# Link ws2_32 library for Winsock functions
target_link_libraries(Client ws2_32)
