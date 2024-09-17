cmake_minimum_required(VERSION 3.5)
project(GTKapp)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(ENV{PKG_CONFIG_PATH} "C:/msys64/mingw64/lib/pkgconfig")

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED gtkmm-4.0)

include_directories(${GTK4_INCLUDE_DIRS})
link_directories(${GTK4_LIBRARY_DIRS})

add_definitions(${GTK4_CFLAGS_OTHER})

add_executable(App main.cpp)

set_target_properties(App PROPERTIES WIN32_EXECUTABLE true)

target_link_libraries(App ${GTK4_LIBRARIES})

set(LOG_DIR "${CMAKE_SOURCE_DIR}/bin")
add_definitions(-DLOG_DIR=\"${LOG_DIR}\")
