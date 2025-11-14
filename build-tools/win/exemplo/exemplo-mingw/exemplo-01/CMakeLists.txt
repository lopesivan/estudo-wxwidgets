cmake_minimum_required(VERSION 3.20)
project(main LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(WX_PREFIX "C:/Users/ivan/source/repos/SDKs/MinGW64/wx-3.3.1-gcc-x64")
set(WX_BIN_DIR "${WX_PREFIX}/bin/gcc_x64_dll")

# MUITO IMPORTANTE: use o modo CONFIG (não o FindwxWidgets antigo)
find_package(wxWidgets CONFIG REQUIRED COMPONENTS core base)

add_executable(main WIN32 main.cpp main.exe.manifest)
target_link_libraries(main PRIVATE wx::core wx::base)

# eof
