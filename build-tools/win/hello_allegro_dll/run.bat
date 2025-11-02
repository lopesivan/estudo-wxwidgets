@echo off
setlocal

set "WX_BIN=C:/Users/ivan/source/repos/SDKs/MinGW64/allegro/bin"
set "MINGW_BIN=C:\mingw64\bin"
set "APP_DIR=build"
set "APP=%APP_DIR%\allegro_app.exe"

rem Adiciona as pastas de DLLs ao PATH *somente durante este script*
set "PATH=%WX_BIN%;%MINGW_BIN%;%PATH%"

if not exist "%APP%" (
  echo [ERRO] Nao encontrei o executavel: %APP%
  exit /b 1
)

echo [RUN] Executando %APP% ...
pushd "%APP_DIR%"
allegro_app.exe
set "RC=%ERRORLEVEL%"
popd

endlocal & exit /b %RC%
