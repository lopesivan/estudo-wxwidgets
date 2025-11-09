#!/usr/bin/env bash

# 1) Gere o Makefile com o bakefile (ok)
PYENV_VERSION=2.7.18 \
    LD_LIBRARY_PATH=/home/ivan/wx/xml2/lib \
    /home/ivan/wx/linux-wx-3.2.4/bin/bakefile -f gnu \
    minimal.bkl -o Makefile

# 2) Prepare diretório de build
mkdir -p build
mv -f Makefile build/
cd build

# 3) USE o wx-config de windows (não o de windows!)
wx_config=/home/ivan/wx/windows-wx-3.2.4/bin/wx-config

# (Opcional) mostre configs disponíveis para depuração:
# "$wx_config" --list
# "$wx_config" --selected-config

# 4) CXXFLAGS precisa dos includes do wx
CXXFLAGS="$("$wx_config" --cxxflags)"

# 5) Forçar porta e versão para evitar o bug do --selected-config
#    (wx 3.2 => "32": major=3, minor=2)
WX_PORT=msw
WX_VERSION=32

CXX="$("$wx_config" --cxx)"
CC="$("$wx_config" --cc)"

# 6) Build com Makefile gerado
make \
    CXX="${CXX}" CC="${CC}" \
    WX_CONFIG="${wx_config}" \
    CXXFLAGS="${CXXFLAGS}" \
    WX_PORT="${WX_PORT}" \
    WX_VERSION="${WX_VERSION}"

# Observação: certifique-se de que o destino existe (gccmsw/) antes de copiar as DLLs.
# O binário geralmente sai em gccmsw_dll/ (ex.: minimal.exe). Se necessário, ajuste o destino.
mingw32=/usr/lib/gcc/x86_64-w64-mingw32
V=13
posix=${mingw32}/$V-posix
win32=${mingw32}/$V-win32
w=$posix
w=$win32
d=gccmsw
for dll in libgcc_s_seh-1.dll libstdc++-6.dll; do
    echo cp ${w}/${dll} $d
    cp ${w}/${dll} $d
done

tree $d
echo wine build/gccmsw/minimal.exe

exit 0
