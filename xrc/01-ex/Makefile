# =========================
# Fonte e alvo
# =========================
PROGRAM     ?= app

# Se EMBED_XRC=1, ui_xrc.cpp é gerado e entra no build
EMBED_XRC   ?= 0
XRC_FILE    ?= ui.xrc
XRC_CPP     ?= ui_xrc.cpp

# Detecta fontes .cpp (e soma ui_xrc.cpp se EMBED_XRC=1)
BASE_SOURCES := $(wildcard *.cpp)
ifeq ($(EMBED_XRC),1)
  # remove ui_xrc.cpp da var base (caso exista) e garante ordem
  BASE_SOURCES := $(filter-out $(XRC_CPP), $(BASE_SOURCES))
  SOURCES      := $(BASE_SOURCES) $(XRC_CPP)
else
  SOURCES      := $(BASE_SOURCES)
endif

OBJS        := $(SOURCES:.cpp=.o)
DEPS        := $(OBJS:.o=.d)

# =========================
# Configuração do wxWidgets
# =========================
V              ?= 3.2.4
OS             ?= linux
WX_BASE_DIR    ?= $(HOME)/wx
WX_SRC_DIR     ?= $(WX_BASE_DIR)/wxWidgets-$(V)-$(OS)
WX_INSTALL_DIR ?= $(WX_BASE_DIR)/$(OS)-wx-$(V)
WX_LIB_PATH    ?= $(WX_INSTALL_DIR)/lib

# wx-config e wxrc (permitem override pelo ambiente)
WX_CONFIG      ?= $(WX_INSTALL_DIR)/bin/wx-config
WXRC           ?= $(WX_INSTALL_DIR)/bin/wxrc

# Opções do wx-config
WX_TOOLKIT     ?= gtk3
WX_UNICODE     ?= yes
WX_STATIC      ?= no
WX_VERSION     ?= 3.2
WX_CONFIG_OPT  := --unicode=$(WX_UNICODE) --static=$(WX_STATIC) --toolkit=$(WX_TOOLKIT) --version=$(WX_VERSION)

# =========================
# Toolchain e flags
# =========================
CXX            ?= $(shell $(WX_CONFIG) --cxx)
OPTIMIZE       ?= -O3
DEBUG          ?= 0        # use DEBUG=1 para build de debug
RPATH          ?= 1        # use RPATH=0 para desabilitar rpath

# Flags vindas do wx-config
WX_CXXFLAGS    := $(shell $(WX_CONFIG) --cxxflags $(WX_CONFIG_OPT))
# Inclua std,xrc para garantir o módulo XRC
WX_LDLIBS      := $(shell $(WX_CONFIG) $(WX_CONFIG_OPT) --libs std,xrc)

# Dependências automáticas
DEPFLAGS       := -MMD -MP

# Ajuste de flags por modo
ifeq ($(DEBUG),1)
  CXXFLAGS    ?= -O0 -g
else
  CXXFLAGS    ?= $(OPTIMIZE)
endif

# rpath opcional para libs locais do wx
ifeq ($(RPATH),1)
  LDFLAGS     ?= -Wl,-rpath,$(WX_LIB_PATH)
else
  LDFLAGS     ?=
endif

# Compose final
CXXFLAGS      += $(WX_CXXFLAGS) $(DEPFLAGS) $(WARNINGS)
LDLIBS        += $(WX_LDLIBS)
LINK.o         = $(CXX) $(LDFLAGS)

# =========================
# Alvos
# =========================
.PHONY: all run ldd readelf clean embed help

all: $(PROGRAM)

$(PROGRAM): $(OBJS)
	$(LINK.o) $^ $(LDLIBS) -o $@

# Geração opcional do arquivo embutido do XRC
# (somente quando EMBED_XRC=1)
ifeq ($(EMBED_XRC),1)
$(XRC_CPP): $(XRC_FILE)
	$(WXRC) $< -c -o $@
embed:
	@$(MAKE) EMBED_XRC=1 all
else
embed:
	@echo "Use EMBED_XRC=1 para embutir o XRC (ex.: make EMBED_XRC=1)"
endif

run: $(PROGRAM)
	./$(PROGRAM)

ldd: $(PROGRAM)
	ldd $^ | grep libwx_ || true

readelf: $(PROGRAM)
	readelf -d $^ | grep -E "(RPATH|RUNPATH)" || true

clean:
	/bin/rm -rf $(PROGRAM) $(OBJS) $(DEPS) $(XRC_CPP)

# Dependências automáticas
-include $(DEPS)

help:
	@echo "Alvos úteis:"
	@echo "  make               -> compila com XRC externo (carrega ui.xrc em runtime)"
	@echo "  make EMBED_XRC=1   -> embute o XRC (gera ui_xrc.cpp com wxrc -c)"
	@echo "  make run           -> executa ./$(PROGRAM)"
	@echo "  make clean         -> limpa artefatos"
