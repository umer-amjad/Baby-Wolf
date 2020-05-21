cmake_minimum_required(VERSION 3.10)
project(Baby-Wolf)
set(CMAKE_CXX_STANDARD 17)
file(GLOB src *.cpp)

find_package(X11 REQUIRED)

message(STATUS "X11_FOUND = ${X11_FOUND}")
message(STATUS "X11_INCLUDE_DIR = ${X11_INCLUDE_DIR}")
message(STATUS "X11_LIBRARIES = ${X11_LIBRARIES}")

include_directories(${X11_INCLUDE_DIR})

link_directories(${X11_LIBRARIES})

add_executable(Baby-Wolf-Test ${src})

target_link_libraries(Baby-Wolf-Test ${X11_LIBRARIES})
