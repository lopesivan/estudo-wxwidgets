cmake_minimum_required(VERSION 3.20)
project(wx_hello LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(WX_PREFIX "C:/Users/ivan/source/repos/SDKs/MinGW64/wx-3.3.1-gcc-x64")
set(WX_BIN_DIR "${WX_PREFIX}/bin/gcc_x64_dll")

# MUITO IMPORTANTE: use o modo CONFIG (não o FindwxWidgets antigo)
find_package(wxWidgets CONFIG REQUIRED COMPONENTS core base)

add_executable(wx_hello WIN32 main.cpp)
target_link_libraries(wx_hello PRIVATE wx::core wx::base)

add_custom_target(
  run
  COMMAND ${CMAKE_COMMAND} -E env "PATH=${WX_BIN_DIR};$ENV{PATH}"
          $<TARGET_FILE:wx_hello>
  DEPENDS wx_hello
  WORKING_DIRECTORY $<TARGET_FILE_DIR:wx_hello>)
