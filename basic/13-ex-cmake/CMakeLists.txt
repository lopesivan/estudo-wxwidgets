cmake_minimum_required(VERSION 3.16)
project(BouncingBalls)

# Padrões do C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Opções de build para debug
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Especificar a localização do wxWidgets
set(wxWidgets_ROOT_DIR "/home/ivan/wx/linux-wx-3.3.1")
set(wxWidgets_CONFIG_OPTIONS --prefix=${wxWidgets_ROOT_DIR})

# Encontrar wxWidgets
find_package(wxWidgets REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})

add_executable(BouncingBalls main.cpp)
target_link_libraries(BouncingBalls ${wxWidgets_LIBRARIES})

# Adicionar includes explicitamente se necessário
target_include_directories(BouncingBalls PRIVATE ${wxWidgets_INCLUDE_DIRS})
target_compile_options(BouncingBalls PRIVATE ${wxWidgets_CXX_FLAGS})
