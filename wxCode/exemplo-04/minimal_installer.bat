@echo off
REM ==========================================
REM Script para compilar e criar instalador
REM do Minimal Application
REM ==========================================
SETLOCAL EnableDelayedExpansion

echo ==========================================
echo   Build e Instalador - Minimal App
echo ==========================================
echo.

REM ==========================================
REM CONFIGURAÇÕES
REM ==========================================
set APP_NAME=minimal
set INNO_SETUP="C:\Program Files (x86)\Inno Setup 6\ISCC.exe"
set BUILD_DIR=gccmsw
set INSTALLER_SCRIPT=minimal_installer.iss
set OUTPUT_DIR=installer_output

REM ==========================================
REM VERIFICAR INNO SETUP
REM ==========================================
echo [1/5] Verificando Inno Setup...
if not exist %INNO_SETUP% (
    echo.
    echo ERRO: Inno Setup nao encontrado!
    echo.
    echo Instale o Inno Setup de: https://jrsoftware.org/isdl.php
    echo Ou ajuste o caminho na variavel INNO_SETUP neste script.
    echo.
    pause
    exit /b 1
)
echo    OK - Inno Setup encontrado

REM ==========================================
REM VERIFICAR ARQUIVOS NECESSÁRIOS
REM ==========================================
echo [2/5] Verificando arquivos necessarios...

if not exist "%BUILD_DIR%\%APP_NAME%.exe" (
    echo.
    echo ERRO: Executavel nao encontrado: %BUILD_DIR%\%APP_NAME%.exe
    echo.
    echo Execute o script de compilacao primeiro:
    echo    ./build.sh
    echo.
    pause
    exit /b 1
)
echo    OK - %APP_NAME%.exe encontrado

if not exist "%BUILD_DIR%\libgcc_s_seh-1.dll" (
    echo    AVISO: libgcc_s_seh-1.dll nao encontrada!
)

if not exist "%BUILD_DIR%\libstdc++-6.dll" (
    echo    AVISO: libstdc++-6.dll nao encontrada!
)

if not exist "%INSTALLER_SCRIPT%" (
    echo.
    echo ERRO: Script do instalador nao encontrado: %INSTALLER_SCRIPT%
    echo.
    pause
    exit /b 1
)
echo    OK - Script do instalador encontrado

REM ==========================================
REM LISTAR ARQUIVOS QUE SERÃO INSTALADOS
REM ==========================================
echo [3/5] Arquivos que serao incluidos no instalador:
dir /B "%BUILD_DIR%\*.exe" "%BUILD_DIR%\*.dll" 2>nul | findstr /V ".o .d" | findstr "."
if errorlevel 1 (
    echo    Nenhum arquivo encontrado!
    pause
    exit /b 1
)

REM ==========================================
REM CRIAR DIRETÓRIO DE SAÍDA
REM ==========================================
echo [4/5] Preparando diretorios...
if not exist "%OUTPUT_DIR%" mkdir "%OUTPUT_DIR%"
echo    OK - Diretorio de saida: %OUTPUT_DIR%

REM ==========================================
REM COMPILAR INSTALADOR
REM ==========================================
echo [5/5] Compilando instalador com Inno Setup...
echo.
%INNO_SETUP% "%INSTALLER_SCRIPT%"

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo ==========================================
    echo   ERRO: Falha ao criar instalador!
    echo ==========================================
    echo.
    echo Verifique:
    echo   1. Se o arquivo %INSTALLER_SCRIPT% esta correto
    echo   2. Se os caminhos dos arquivos estao corretos
    echo   3. As mensagens de erro acima
    echo.
    pause
    exit /b 1
)

REM ==========================================
REM SUCESSO
REM ==========================================
echo.
echo ==========================================
echo   INSTALADOR CRIADO COM SUCESSO!
echo ==========================================
echo.
echo Arquivo gerado:
for %%f in ("%OUTPUT_DIR%\*.exe") do (
    echo   %%~nxf
    echo   Tamanho: %%~zf bytes
    echo   Caminho completo: %%~ff
)
echo.
echo ==========================================
echo   PROXIMOS PASSOS
echo ==========================================
echo.
echo Para testar o instalador:
echo   1. Execute: %OUTPUT_DIR%\minimal-setup-*.exe
echo   2. Siga o assistente de instalacao
echo.
echo Para testar em outra maquina:
echo   1. Copie o arquivo .exe para a maquina
echo   2. Execute como administrador
echo.
echo Para criar versao portavel (sem instalador):
echo   1. Copie a pasta %BUILD_DIR% para onde desejar
echo   2. Execute minimal.exe diretamente
echo.
echo ==========================================

REM ==========================================
REM PERGUNTAR SE DESEJA EXECUTAR
REM ==========================================
echo.
set /p CHOICE="Deseja executar o instalador agora? (S/N): "
if /i "%CHOICE%"=="S" (
    echo.
    echo Executando instalador...
    for %%f in ("%OUTPUT_DIR%\*.exe") do (
        start "" "%%f"
    )
) else (
    echo.
    echo Instalador nao executado.
)

echo.
echo Pressione qualquer tecla para sair...
pause >nul

