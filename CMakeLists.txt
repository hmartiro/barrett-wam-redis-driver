cmake_minimum_required(VERSION 2.8.4)
project(barrett-wam-redis-driver)

# Use Release if no configuration specified
if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fPIC -Wall")

# Print out compiler commands
# set(CMAKE_VERBOSE_MAKEFILE ON)

# ---------------------------------------------------------
# Build target
# ---------------------------------------------------------

set(SRC main.cpp wam_redis_driver.cpp)

add_executable(wam_redis_driver ${SRC})

## libbarrett
find_package(Barrett REQUIRED)
include_directories(${BARRETT_INCLUDE_DIRS})
link_directories(${BARRETT_LIBRARY_DIRS})
add_definitions(${BARRETT_DEFINITIONS})
target_link_libraries(wam_redis_driver ${BARRETT_LIBRARIES})

# ---------------------------------------------------------
# Find and link libraries
# ---------------------------------------------------------

# Redox, a C++ client for Redis
find_library(REDOX_LIB NAMES redox)
target_link_libraries(wam_redis_driver ${REDOX_LIB})

find_library(JSONCPP_LIB NAMES jsoncpp)
target_link_libraries(wam_redis_driver ${JSONCPP_LIB})

# Barrett WAM driver
# find_library(BARRETT_LIB NAMES barrett)
# target_link_libraries(wam_redis_driver ${BARRETT_LIB})
