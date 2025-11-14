# mesmos caminhos que você usa
set(WX_BIN
    "C:/Users/ivan/source/repos/SDKs/MinGW64/wx-3.3.1-gcc-x64/bin/gcc_x64_dll")
set(MINGW_BIN "C:/mingw64/bin")

set(RUN_BAT "${CMAKE_BINARY_DIR}/run_${PROJECT_NAME}.bat")
file(WRITE "${RUN_BAT}"
     "@echo off\r\n" "set \"PATH=${WX_BIN};${MINGW_BIN};%PATH%\"\r\n"
     "start \"\" \"%~dp0\\${PROJECT_NAME}.exe\"\r\n")

add_custom_target(
  run
  COMMAND "${RUN_BAT}"
  WORKING_DIRECTORY "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
  DEPENDS ${PROJECT_NAME}
  USES_TERMINAL VERBATIM
  COMMENT "Executando ${PROJECT_NAME} via .bat (sem problemas de escape)…")

# eof
