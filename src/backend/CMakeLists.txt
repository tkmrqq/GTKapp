cmake_minimum_required(VERSION 3.5)

project(BatteryBackend)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(${PROJECT_SOURCE_DIR})

add_executable(battery_app
    battery.cpp
    batteryInfo.hpp batteryInfo.cpp
)

target_link_libraries(battery_app PowrProf)
target_link_libraries(battery_app setupapi)

set(LOG_DIR "${CMAKE_SOURCE_DIR}/bin")

add_definitions(-DLOG_DIR=\"${LOG_DIR}\")
