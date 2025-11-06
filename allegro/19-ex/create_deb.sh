#!/bin/bash
# Script para criar pacote .deb para myapp

set -e # Sai se houver erro

# ============================================
# Configurações do pacote
# ============================================
APP_NAME="app"
APP_VERSION="1.0.0"
ARCH="amd64" # ou "i386", "armhf", "arm64"
MAINTAINER="$(git config --get user.name) <$(git config --get user.email)>"
DESCRIPTION="Descrição curta do aplicativo"
LONG_DESCRIPTION="Descrição mais detalhada do aplicativo.
 Pode ter múltiplas linhas."

# ============================================
# Variáveis de diretório
# ============================================
BUILD_DIR="deb_build"
PKG_NAME="${APP_NAME}_${APP_VERSION}_${ARCH}"
PKG_DIR="${BUILD_DIR}/${PKG_NAME}"
DEBIAN_DIR="${PKG_DIR}/DEBIAN"
INSTALL_DIR="${PKG_DIR}/usr/local/bin"

# ============================================
# Limpeza e criação de estrutura
# ============================================
echo "Limpando diretório de build anterior..."
rm -rf "${BUILD_DIR}"

echo "Criando estrutura de diretórios..."
mkdir -p "${DEBIAN_DIR}"
mkdir -p "${INSTALL_DIR}"

# ============================================
# Copiar binário compilado
# ============================================
echo "Copiando binário..."
if [ ! -f "${APP_NAME}" ]; then
    echo "ERRO: Binário '${APP_NAME}' não encontrado!"
    echo "Compile o aplicativo antes de executar este script."
    exit 1
fi

cp "${APP_NAME}" "${INSTALL_DIR}/"
chmod 755 "${INSTALL_DIR}/${APP_NAME}"

# ============================================
# Criar arquivo control
# ============================================
echo "Criando arquivo control..."
cat >"${DEBIAN_DIR}/control" <<EOF
Package: ${APP_NAME}
Version: ${APP_VERSION}
Architecture: ${ARCH}
Maintainer: ${MAINTAINER}
Priority: optional
Section: utils
Homepage: https://github.com/$(git config --get github.lopesivan.user)/${APP_NAME}
Description: ${DESCRIPTION}
 ${LONG_DESCRIPTION}
EOF

# ============================================
# Criar script postinst (opcional)
# ============================================
echo "Criando script postinst..."
cat >"${DEBIAN_DIR}/postinst" <<'EOF'
#!/bin/bash
set -e

echo "Instalação de ${APP_NAME} concluída!"
echo "Execute '${APP_NAME}' para iniciar o aplicativo."

exit 0
EOF
chmod 755 "${DEBIAN_DIR}/postinst"

# ============================================
# Criar script prerm (opcional)
# ============================================
echo "Criando script prerm..."
cat >"${DEBIAN_DIR}/prerm" <<'EOF'
#!/bin/bash
set -e

echo "Removendo ${APP_NAME}..."

exit 0
EOF
chmod 755 "${DEBIAN_DIR}/prerm"

# ============================================
# Criar arquivo copyright (opcional mas recomendado)
# ============================================
DOC_DIR="${PKG_DIR}/usr/share/doc/${APP_NAME}"
mkdir -p "${DOC_DIR}"

cat >"${DOC_DIR}/copyright" <<EOF
Format: https://www.debian.org/doc/packaging-manuals/copyright-format/1.0/
Upstream-Name: ${APP_NAME}
Source: https://github.com/$(git config --get github.lopesivan.user)/${APP_NAME}

Files: *
Copyright: $(date +%Y) Seu Nome
License: MIT
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 .
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 .
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
EOF

# ============================================
# Criar changelog (opcional mas recomendado)
# ============================================
cat >"${DOC_DIR}/changelog.Debian" <<EOF
${APP_NAME} (${APP_VERSION}) unstable; urgency=low

  * Versão inicial do pacote

 -- ${MAINTAINER}  $(date -R)
EOF
gzip -9 "${DOC_DIR}/changelog.Debian"

# ============================================
# Calcular dependências (opcional)
# ============================================
echo "Detectando dependências..."
DEPS=$(ldd "${APP_NAME}" 2>/dev/null | grep "=>" | awk '{print $3}' | xargs -r dpkg -S 2>/dev/null | cut -d: -f1 | sort -u | tr '\n' ',' | sed 's/,$//' || true)

if [ -n "$DEPS" ]; then
    echo "Depends: ${DEPS}" >>"${DEBIAN_DIR}/control"
    echo "Dependências detectadas: ${DEPS}"
fi

# ============================================
# Construir o pacote .deb
# ============================================
echo "Construindo pacote .deb..."
fakeroot dpkg-deb --build "${PKG_DIR}"

# ============================================
# Mover para diretório atual
# ============================================
mv "${BUILD_DIR}/${PKG_NAME}.deb" .
echo ""
echo "=========================================="
echo "Pacote criado com sucesso!"
echo "Arquivo: ${PKG_NAME}.deb"
echo "=========================================="
echo ""
echo "Para instalar, execute:"
echo "  sudo dpkg -i ${PKG_NAME}.deb"
echo ""
echo "Para desinstalar, execute:"
echo "  sudo dpkg -r ${APP_NAME}"
echo ""
echo "Para verificar o conteúdo do pacote:"
echo "  dpkg -c ${PKG_NAME}.deb"
echo ""
echo "Para ver informações do pacote:"
echo "  dpkg -I ${PKG_NAME}.deb"
echo "=========================================="

# ============================================
# Limpeza (opcional)
# ============================================
read -p "Deseja limpar os arquivos temporários? (s/N): " -n 1 -r
echo
if [[ $REPLY =~ ^[Ss]$ ]]; then
    rm -rf "${BUILD_DIR}"
    echo "Arquivos temporários removidos."
fi

exit 0
