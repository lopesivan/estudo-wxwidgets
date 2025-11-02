@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvarsall.bat" x86



@echo off
if exist "build-msvc-x64\" (
    rmdir /s /q "build-msvc-x64"
    echo Diretorio build-msvc-x64 removido.
) else (
    echo Diretorio build-msvc-x64 nao existe.
)
