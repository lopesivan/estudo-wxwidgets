cmake_minimum_required(VERSION 3.15)
project(WxWidgetsDemo CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Encontra o wxWidgets instalado pelo Conan
find_package(wxWidgets REQUIRED CONFIG)

# Cria o executável
add_executable(wx_demo main.cpp)

# Linka as bibliotecas do wxWidgets
target_link_libraries(wx_demo PRIVATE wxWidgets::wxWidgets)
