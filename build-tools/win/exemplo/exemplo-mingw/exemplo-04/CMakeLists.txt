cmake_minimum_required(VERSION 3.20)
project(minimal LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(WX_PREFIX "C:/Users/ivan/source/repos/SDKs/MinGW64/wx-3.3.1-gcc-x64")
set(WX_BIN_DIR "${WX_PREFIX}/bin/gcc_x64_dll")

# MUITO IMPORTANTE: use o modo CONFIG (não o FindwxWidgets antigo)
find_package(wxWidgets CONFIG REQUIRED COMPONENTS core base)

add_executable(minimal WIN32 minimal.cpp minimal.exe.manifest)
target_link_libraries(minimal PRIVATE wx::core wx::base)

# eof
