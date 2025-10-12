cmake_minimum_required(VERSION 3.18)

message(STATUS "Building SUN")

set(wxWidgets_USE_DEBUG ON)
set(wxWidgets_USE_UNICODE ON)
find_package(wxWidgets REQUIRED net gl core base)

include(${wxWidgets_USE_FILE})

add_executable(app)

target_sources(app PRIVATE
        src/app.cpp)

target_link_libraries(app PRIVATE ${wxWidgets_LIBRARIES})


