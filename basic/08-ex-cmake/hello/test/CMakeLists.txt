# BuildGTest Unit Testing Binary
#
# Note: This needs to be triggered with add_subdirectory() from main project CMakeLists
# with SRC and LINK_LIBS populated with project's source files/libraries
#
# Building GTest binary from main CMakeLists causes duplicate compile commands
# with the gtest includes/libs to end up in the intellisense export.
#
# https://gitlab.kitware.com/cmake/cmake/-/merge_requests/5651
# This merge request to allow specifying a single target for intellisense exports
# looks like it's almost ready to be merged.  Once a new CMake release includes it
# this file could all be moved to the main CMakeLists.txt.in if desired.
#

cmake_minimum_required(VERSION 3.18)
message(STATUS "Building PlaceHolderUnitTest")

if (MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/W4 /WX)
else()
    # lots of warnings and all warnings as errors
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

# Setup GoogleTest
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
        ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
        EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
#if (CMAKE_VERSION VERSION_LESS 2.8.11)
#  include_directories("${gtest_SOURCE_DIR}/include")
#endif()

add_executable(TestPlaceHolderUnitTest TestPlaceHolderUnitTest.cpp)

target_link_libraries(TestPlaceHolderUnitTest gtest_main)

add_gtest(TestPlaceHolderUnitTest)