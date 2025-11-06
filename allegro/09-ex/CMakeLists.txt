cmake_minimum_required(VERSION 3.10)
project(allegro_wx)

# Padrões do C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Opções de build para debug
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(wxWidgets REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})

find_package(PkgConfig REQUIRED)
pkg_check_modules(ALLEGRO REQUIRED allegro-5 allegro_main-5
                  allegro_primitives-5)

add_executable(allegro_wx main.cpp)

target_include_directories(allegro_wx PRIVATE ${ALLEGRO_INCLUDE_DIRS})
target_link_libraries(allegro_wx ${wxWidgets_LIBRARIES} ${ALLEGRO_LIBRARIES})
