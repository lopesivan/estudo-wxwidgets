cmake_minimum_required(VERSION 3.16)
project(wx_allegro_paint)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(wxWidgets 3.2 REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})

find_package(PkgConfig REQUIRED)
pkg_check_modules(ALLEGRO REQUIRED
    allegro-5
    allegro_primitives-5)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

add_executable(${PROJECT_NAME} src/main.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${wxWidgets_INCLUDE_DIRS}
    ${ALLEGRO_INCLUDE_DIRS}
    ${GTK3_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${wxWidgets_LIBRARIES}
    ${ALLEGRO_LIBRARIES}
    ${GTK3_LIBRARIES})

target_compile_options(${PROJECT_NAME} PRIVATE
    ${wxWidgets_CFLAGS}
    ${ALLEGRO_CFLAGS_OTHER}
    ${GTK3_CFLAGS_OTHER})

find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)
