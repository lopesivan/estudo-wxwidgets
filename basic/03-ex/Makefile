# =========================
# Configuração do projeto
# =========================
PROGRAM       ?= app
SRC_DIR       ?= .
BUILD_DIR     ?= build
BIN_DIR       ?= bin

# Descobre todos os .cpp no diretório de código
SRCS          := $(wildcard $(SRC_DIR)/*.cpp)
OBJS          := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))
DEPS          := $(OBJS:.o=.d)

# =========================
# Configuração do wxWidgets
# =========================
V             ?= 3.2.4
WX_BASE_DIR   ?= $(HOME)/wx
WX_SRC_DIR    ?= $(WX_BASE_DIR)/wxWidgets-$(V)-linux
WX_INSTALL_DIR?= $(WX_BASE_DIR)/linux-wx-$(V)
WX_LIB_PATH   ?= $(WX_INSTALL_DIR)/lib

# Permita sobrescrever WX_CONFIG via ambiente, senão use o padrão
WX_CONFIG     ?= $(WX_INSTALL_DIR)/bin/wx-config

# Opções do wx-config (ajuste toolkit/unicode/static se necessário)
WX_TOOLKIT    ?= gtk3
WX_UNICODE    ?= yes
WX_STATIC     ?= no
WX_VERSION    ?= 3.2
WX_CONFIG_OPT := --unicode=$(WX_UNICODE) --static=$(WX_STATIC) --toolkit=$(WX_TOOLKIT) --version=$(WX_VERSION)

# =========================
# Toolchain e flags
# =========================
CXX           ?= $(shell $(WX_CONFIG) --cxx)
WARNINGS      ?= -Wall -Wextra -Wpedantic
OPTIMIZE      ?= -O3
DEBUG         ?= 0            # use DEBUG=1 para build de debug
RPATH         ?= 1            # use RPATH=0 para desabilitar rpath

# Flags vindas do wx-config
WX_CXXFLAGS   := $(shell $(WX_CONFIG) --cxxflags $(WX_CONFIG_OPT))
WX_LDLIBS     := $(shell $(WX_CONFIG) $(WX_CONFIG_OPT) --libs core,base)

# Flags de dep (-MMD -MP) para gerar .d e recompilar só o necessário
DEPFLAGS      := -MMD -MP

# Ajuste de flags por modo
ifeq ($(DEBUG),1)
  CXXFLAGS    ?= -O0 -g
  CPPFLAGS    ?=
else
  CXXFLAGS    ?= $(OPTIMIZE)
  CPPFLAGS    ?=
endif

# rpath opcional apontando para as libs do wx instaladas localmente
ifeq ($(RPATH),1)
  LDFLAGS     ?= -Wl,-rpath,$(WX_LIB_PATH)
else
  LDFLAGS     ?=
endif

# Compose final
CXXFLAGS      += $(WX_CXXFLAGS) $(WARNINGS)
LDLIBS        += $(WX_LDLIBS)

# =========================
# Alvos
# =========================
.PHONY: all clean run info

all: prep $(BIN_DIR)/$(PROGRAM)

# Link final
$(BIN_DIR)/$(PROGRAM): $(OBJS)
	@echo "  LINK    $@"
	$(CXX) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS)

# Compilação por arquivo
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "  CXX     $<"
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(DEPFLAGS) -c $< -o $@

# Diretórios
prep:
	@mkdir -p $(BUILD_DIR) $(BIN_DIR)

# Executar o programa
run: all
	@echo "  RUN     $(BIN_DIR)/$(PROGRAM)"
	@$(BIN_DIR)/$(PROGRAM)

# Info útil para depuração
info:
	@echo "PROGRAM      = $(PROGRAM)"
	@echo "SRCS         = $(SRCS)"
	@echo "OBJS         = $(OBJS)"
	@echo "CXX          = $(CXX)"
	@echo "WX_CONFIG    = $(WX_CONFIG)"
	@echo "CXXFLAGS     = $(CXXFLAGS)"
	@echo "LDFLAGS      = $(LDFLAGS)"
	@echo "LDLIBS       = $(LDLIBS)"

# Limpeza
clean:
	@echo "  CLEAN"
	@rm -rf $(BUILD_DIR) $(BIN_DIR)

# Dependências automáticas
-include $(DEPS)
