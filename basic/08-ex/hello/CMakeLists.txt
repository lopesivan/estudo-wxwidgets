cmake_minimum_required(VERSION 3.20)

message(STATUS "Building SUN")

set(wxWidgets_USE_DEBUG ON)
set(wxWidgets_USE_UNICODE ON)

IF(CMAKE_BUILD_TYPE MATCHES Release)
    message(STATUS "Release mode" )
    set(wxWidgets_ROOT_DIR C:/wx314_install/Release)
    set(wxWidgets_LIB_DIR C:/wx314_install/Release/lib/vc_x64_lib)
    set(wxWidgets_INCLUDE_DIRS C:/wx314_install/Release/include)
ENDIF()

IF(CMAKE_BUILD_TYPE MATCHES Debug)
    message(STATUS "Debug mode" )
    set(wxWidgets_ROOT_DIR "C:/wx314_install/Debug")
    set(wxWidgets_LIB_DIR C:/wx314_install/Debug/lib/vc_x64_lib)
    set(wxWidgets_INCLUDE_DIRS C:/wx314_install/Debug/include)
ENDIF()


message(STATUS "wxWidgets location ${wxWidgets_ROOT_DIR}")
# Note that for MinGW users the order of libs is important!

find_package(wxWidgets REQUIRED net gl core base)

include(${wxWidgets_USE_FILE})

add_executable(app WIN32)

target_sources(app PRIVATE
        src/app.cpp)

target_link_libraries(app PRIVATE ${wxWidgets_LIBRARIES})


