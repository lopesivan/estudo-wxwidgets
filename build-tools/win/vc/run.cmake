# mesmos caminhos que você usa

# --- Caminhos do wxWidgets (MSVC) ---
# Pasta das DLLs (carregadas em runtime pelo PATH)
set(WX_BIN
    "C:/Users/ivan/source/repos/SDKs/VC/wx-3.3.1-msvc-x64/bin/vc_x64_dll"
    CACHE PATH "")

# Alvo a rodar; por padrão usa o nome do projeto (o target criado por
# add_executable)
if(NOT DEFINED RUN_TARGET)
  set(RUN_TARGET "${PROJECT_NAME}")
endif()

set(RUN_BAT "${CMAKE_BINARY_DIR}/run_${PROJECT_NAME}.bat")
file(WRITE "${RUN_BAT}"
     "@echo off\r\n" "set \"PATH=${WX_BIN};${MINGW_BIN};%PATH%\"\r\n"
     "start \"\" \"%~dp0\\Release\\${PROJECT_NAME}.exe\"\r\n")

add_custom_target(
  run
  COMMAND "${RUN_BAT}"
  WORKING_DIRECTORY "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
  DEPENDS ${PROJECT_NAME}
  USES_TERMINAL VERBATIM
  COMMENT "Executando ${PROJECT_NAME} via .bat (sem problemas de escape)…")

# eof
