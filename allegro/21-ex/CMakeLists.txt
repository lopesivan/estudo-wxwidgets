cmake_minimum_required(VERSION 3.16)
project(wx_allegro_paint)

set(CMAKE_CXX_STANDARD 17)

# ---------- wxWidgets ----------
find_package(wxWidgets 3.2 REQUIRED COMPONENTS core base gl)
include(${wxWidgets_USE_FILE})

# ---------- Allegro 5 ----------
find_package(PkgConfig REQUIRED)
pkg_check_modules(ALLEGRO REQUIRED
    allegro-5
    allegro_primitives-5
    allegro_font-5
    allegro_ttf-5)

# ---------- executável ----------
add_executable(${PROJECT_NAME}
    src/main.cpp
    src/PaintPanel.hpp
)

target_include_directories(${PROJECT_NAME} PRIVATE ${ALLEGRO_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME}
    ${wxWidgets_LIBRARIES}
    ${ALLEGRO_LIBRARIES}
)
target_compile_options(${PROJECT_NAME} PRIVATE ${ALLEGRO_CFLAGS_OTHER})

# Windows: evita console
if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)
endif()
