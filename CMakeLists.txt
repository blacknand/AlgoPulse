# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(HFTBot VERSION 1.0)

# Set C++ standard to 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Specify the source directory
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Add executable target with multiple source files
add_executable(hftbot 
    ${SOURCE_DIR}/main.cpp
    ${SOURCE_DIR}/HFTBot.cpp
)

# Find Boost (required for networking)
find_package(Boost 1.65 REQUIRED COMPONENTS system)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    target_link_libraries(hftbot PRIVATE ${Boost_LIBRARIES})
else()
    message(FATAL_ERROR "Boost not found. Please install libboost-all-dev.")
endif()

# Find ZeroMQ
find_package(PkgConfig REQUIRED)
pkg_check_modules(ZMQ REQUIRED libzmq)
if(ZMQ_FOUND)
    target_include_directories(hftbot PRIVATE ${ZMQ_INCLUDE_DIRS})
    target_link_libraries(hftbot PRIVATE ${ZMQ_LIBRARIES})
    # Explicitly link against zmq library
    target_link_directories(hftbot PRIVATE ${ZMQ_LIBRARY_DIRS})
else()
    message(FATAL_ERROR "ZeroMQ not found. Please install libzmq3-dev.")
endif()

# Link threading support (required for std::thread)
find_package(Threads REQUIRED)
target_link_libraries(hftbot PRIVATE Threads::Threads)

# Set optimization flags for performance
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(hftbot PRIVATE -O3 -march=native)
endif()

# Include directories for project headers
target_include_directories(hftbot PRIVATE ${SOURCE_DIR})