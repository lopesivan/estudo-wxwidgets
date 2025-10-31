cmake_minimum_required(VERSION 3.16)
project(BouncingBalls)

# Padrões do C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Opções de build para debug
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(wxWidgets REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})

add_executable(BouncingBalls main.cpp)
target_link_libraries(BouncingBalls ${wxWidgets_LIBRARIES})
