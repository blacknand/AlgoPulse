cmake_minimum_required(VERSION 3.10)
project(HFTBot VERSION 1.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Build the hftbot executable
add_executable(hftbot 
    ${SOURCE_DIR}/main.cpp
    ${SOURCE_DIR}/HFTBot.cpp
    ${SOURCE_DIR}/DataIngestion.cpp
)

# Build the publisher executable
add_executable(publisher
    ${SOURCE_DIR}/Publisher.cpp
)

# Find and link Boost, ZeroMQ, and Threads...
find_package(Boost 1.65 REQUIRED COMPONENTS system)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    target_link_libraries(hftbot PRIVATE ${Boost_LIBRARIES})
    target_link_libraries(publisher PRIVATE ${Boost_LIBRARIES})
else()
    message(FATAL_ERROR "Boost not found. Please install libboost-all-dev.")
endif()

find_package(PkgConfig REQUIRED)
pkg_check_modules(ZMQ REQUIRED libzmq)
if(ZMQ_FOUND)
    target_include_directories(hftbot PRIVATE ${ZMQ_INCLUDE_DIRS})
    target_link_libraries(hftbot PRIVATE ${ZMQ_LIBRARIES})
    target_link_directories(hftbot PRIVATE ${ZMQ_LIBRARY_DIRS})

    target_include_directories(publisher PRIVATE ${ZMQ_INCLUDE_DIRS})
    target_link_libraries(publisher PRIVATE ${ZMQ_LIBRARIES})
    target_link_directories(publisher PRIVATE ${ZMQ_LIBRARY_DIRS})
else()
    message(FATAL_ERROR "ZeroMQ not found. Please install libzmq3-dev.")
endif()

find_package(Threads REQUIRED)
target_link_libraries(hftbot PRIVATE Threads::Threads)
target_link_libraries(publisher PRIVATE Threads::Threads)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(hftbot PRIVATE -O3 -march=native)
    target_compile_options(publisher PRIVATE -O3 -march=native)
endif()

target_include_directories(hftbot PRIVATE ${SOURCE_DIR})
target_include_directories(publisher PRIVATE ${SOURCE_DIR})
