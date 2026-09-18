.PHONY: all clean init config build run rebuild help

# Configurações
BUILD_DIR := build
BUILD_TYPE ?= Release
# Conan 2 com cmake_layout coloca os arquivos em build/build/Release/generators
CONAN_BUILD_DIR := $(BUILD_DIR)/build/$(BUILD_TYPE)
GENERATORS_DIR := $(CONAN_BUILD_DIR)/generators
EXECUTABLE := $(CONAN_BUILD_DIR)/wx_demo

# Cores para output
GREEN := \033[0;32m
YELLOW := \033[0;33m
BLUE := \033[0;34m
CYAN := \033[0;36m
MAGENTA := \033[0;35m
NC := \033[0m # No Color

# Target padrão
all: init config build

# Ajuda
help:
	@echo "$(CYAN)╔════════════════════════════════════════════╗$(NC)"
	@echo "$(CYAN)║      wxWidgets Demo - Build System         ║$(NC)"
	@echo "$(CYAN)╚════════════════════════════════════════════╝$(NC)"
	@echo ""
	@echo "$(BLUE)📦 Comandos disponíveis:$(NC)"
	@echo "  $(GREEN)make all$(NC)       - Executa init, config e build"
	@echo "  $(GREEN)make init$(NC)      - Instala dependências com Conan"
	@echo "  $(GREEN)make config$(NC)    - Configura o projeto com CMake"
	@echo "  $(GREEN)make build$(NC)     - Compila o projeto"
	@echo "  $(GREEN)make run$(NC)       - Executa o programa compilado"
	@echo "  $(GREEN)make rebuild$(NC)   - Limpa e reconstrói tudo"
	@echo "  $(GREEN)make clean$(NC)     - Remove arquivos de build"
	@echo ""
	@echo "$(YELLOW)⚙️  Variáveis:$(NC)"
	@echo "  BUILD_TYPE=Release|Debug (padrão: Release)"
	@echo ""
	@echo "$(CYAN)🚀 Exemplo de uso:$(NC)"
	@echo "  make all && make run"
	@echo ""
	@echo "$(MAGENTA)⚠️  ATENÇÃO:$(NC)"
	@echo "  wxWidgets pode demorar ~10-20 min para compilar na primeira vez!"
	@echo "  Tenha paciência... ☕"

# Instala as dependências com Conan
init:
	@echo "$(BLUE)>>> 📦 Instalando dependências com Conan...$(NC)"
	@echo "$(YELLOW)⚠️  IMPORTANTE: wxWidgets é grande e pode demorar para compilar!$(NC)"
	@echo "$(YELLOW)    Primeira compilação: ~10-20 minutos$(NC)"
	@echo "$(YELLOW)    Próximas vezes: instantâneo (usa cache)$(NC)"
	@echo ""
	PKG_CONFIG_PATH=/usr/lib/x86_64-linux-gnu/pkgconfig \
	conan install . \
	--output-folder=$(BUILD_DIR) \
	--build=missing -s build_type=$(BUILD_TYPE)
	@echo "$(GREEN)✓ Dependências instaladas$(NC)"
	@echo "$(YELLOW)ℹ  Arquivos gerados em: $(GENERATORS_DIR)$(NC)"

# Configura o CMake usando as toolchains do Conan
config:
	@if [ ! -f "$(GENERATORS_DIR)/conan_toolchain.cmake" ]; then \
		echo "$(YELLOW)⚠  Toolchain do Conan não encontrado. Execute 'make init' primeiro.$(NC)"; \
		exit 1; \
	fi
	@echo "$(BLUE)>>> ⚙️  Configurando CMake...$(NC)"
	cmake -S . -B $(CONAN_BUILD_DIR) \
		-DCMAKE_TOOLCHAIN_FILE=$(GENERATORS_DIR)/conan_toolchain.cmake \
		-DCMAKE_BUILD_TYPE=$(BUILD_TYPE) \
		-DCMAKE_EXPORT_COMPILE_COMMANDS=ON
	@echo "$(GREEN)✓ CMake configurado$(NC)"

# Compila o projeto
build:
	@if [ ! -f "$(CONAN_BUILD_DIR)/Makefile" ]; then \
		echo "$(YELLOW)⚠  Makefiles do CMake não encontrados. Execute 'make config' primeiro.$(NC)"; \
		exit 1; \
	fi
	@echo "$(BLUE)>>> 🔨 Compilando projeto...$(NC)"
	cmake --build $(CONAN_BUILD_DIR) --config $(BUILD_TYPE) -j$$(nproc)
	@echo "$(GREEN)✓ Compilação concluída$(NC)"
	@echo "$(YELLOW)ℹ  Executável: $(EXECUTABLE)$(NC)"

# Executa o programa
run:
	@if [ ! -f "$(EXECUTABLE)" ]; then \
		echo "$(YELLOW)⚠  Executável não encontrado. Execute 'make build' primeiro.$(NC)"; \
		exit 1; \
	fi
	@echo "$(CYAN)╔════════════════════════════════════════════╗$(NC)"
	@echo "$(CYAN)║      🎨 Iniciando wxWidgets Demo 🎨        ║$(NC)"
	@echo "$(CYAN)╚════════════════════════════════════════════╝$(NC)"
	@echo ""
	@$(EXECUTABLE)

# Reconstrói tudo do zero
rebuild: clean all
	@echo "$(GREEN)✓ Rebuild completo$(NC)"

# Limpa arquivos de build
clean:
	@echo "$(YELLOW)>>> 🧹 Limpando arquivos de build...$(NC)"
	rm -rf $(BUILD_DIR)
	rm -f CMakeUserPresets.json
	@echo "$(GREEN)✓ Limpeza concluída$(NC)"

# Info sobre o projeto
info:
	@echo "$(CYAN)╔════════════════════════════════════════════╗$(NC)"
	@echo "$(CYAN)║         Informações do Projeto             ║$(NC)"
	@echo "$(CYAN)╚════════════════════════════════════════════╝$(NC)"
	@echo ""
	@echo "$(BLUE)Build Type:$(NC) $(BUILD_TYPE)"
	@echo "$(BLUE)Build Dir:$(NC) $(CONAN_BUILD_DIR)"
	@echo "$(BLUE)Executable:$(NC) $(EXECUTABLE)"
	@echo ""
	@echo "$(YELLOW)Estrutura de diretórios:$(NC)"
	@if [ -d "$(BUILD_DIR)" ]; then \
		tree -L 3 $(BUILD_DIR) 2>/dev/null || find $(BUILD_DIR) -maxdepth 3 -type d; \
	else \
		echo "  (build/ não existe ainda - execute 'make init')"; \
	fi

