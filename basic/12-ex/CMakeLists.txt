cmake_minimum_required(VERSION 3.16)
project(BouncingBalls)

find_package(wxWidgets REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})

add_executable(BouncingBalls main.cpp)
target_link_libraries(BouncingBalls ${wxWidgets_LIBRARIES})
