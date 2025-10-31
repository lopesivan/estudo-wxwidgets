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
  BASE_SOURCES := $(filter-out $(XRC_CPP), $(BASE_SOURCES))
  SOURCES      := $(BASE_SOURCES) $(XRC_CPP)
else
  SOURCES      := $(BASE_SOURCES)
endif

OBJS        := $(SOURCES:.cpp=.o)
DEPS        := $(OBJS:.o=.d)

# =========================
# wxWidgets (LOCAL)
# =========================
# Ajuste estes paths para o seu build local:
V              ?= 3.2.4
OS             ?= linux
WX_BASE_DIR    ?= $(HOME)/wx
WX_INSTALL_DIR ?= $(WX_BASE_DIR)/$(OS)-wx-$(V)
WX_LIB_PATH    ?= $(WX_INSTALL_DIR)/lib

WX_CONFIG      ?= $(WX_INSTALL_DIR)/bin/wx-config
WXRC           ?= $(WX_INSTALL_DIR)/bin/wxrc

# Opções do wx-config
WX_STATIC      ?= no
WX_CONFIG_OPT  := --static=$(WX_STATIC)

# =========================
# Toolchain e flags
# =========================
CXX            ?= $(shell $(WX_CONFIG) --cxx)
CXXSTD         ?= -std=c++17
OPTIMIZE       ?= -O3
DEBUG          ?= 0
RPATH          ?= 1        # use RPATH=0 para desabilitar rpath

# Flags vindas do wx-config
WX_CXXFLAGS    := $(shell $(WX_CONFIG) --cxxflags $(WX_CONFIG_OPT))
WX_LDLIBS      := $(shell $(WX_CONFIG) $(WX_CONFIG_OPT) --libs std,xrc)

# Dependências automáticas
DEPFLAGS       := -MMD -MP

# Ajuste de flags por modo
ifeq ($(DEBUG),1)
  CXXFLAGS    ?= -O0 -g
else
  CXXFLAGS    ?= $(OPTIMIZE)
endif

# rpath para garantir uso das libs locais em runtime
ifeq ($(RPATH),1)
  # RPATH "clássico" + ORIGIN; evita RUNPATH (new-dtags)
  LDFLAGS     ?= -Wl,-rpath,$(WX_LIB_PATH) -Wl,-rpath,'$$ORIGIN' -Wl,--disable-new-dtags
else
  LDFLAGS     ?=
endif

# Compose final
CXXFLAGS      += $(CXXSTD) $(WX_CXXFLAGS) $(DEPFLAGS) $(WARNINGS)
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
ifeq ($(EMBED_XRC),1)
$(XRC_CPP): $(XRC_FILE)
	$(WXRC) $< -c -o $@
embed:
	@$(MAKE) EMBED_XRC=1 all
else
embed:
	@echo "Use EMBED_XRC=1 para embutir o XRC (ex.: make EMBED_XRC=1)"
endif

# Prioriza as libs locais na execução
run: $(PROGRAM)
	LD_LIBRARY_PATH="$(WX_LIB_PATH):$$LD_LIBRARY_PATH" ./$(PROGRAM)

ldd: $(PROGRAM)
	ldd $^ | grep wx || true

readelf: $(PROGRAM)
	readelf -d $^ | grep -E "(RPATH|RUNPATH)" || true

clean:
	/bin/rm -rf $(PROGRAM) $(OBJS) $(DEPS) $(XRC_CPP)

# Dependências automáticas
-include $(DEPS)

help:
	@echo "Makefile para wx LOCAL (WX_INSTALL_DIR)"
	@echo "  make               -> compila usando $(WX_CONFIG)"
	@echo "  make EMBED_XRC=1   -> embute XRC (gera ui_xrc.cpp com wxrc -c)"
	@echo "  make run           -> executa com LD_LIBRARY_PATH ajustado"
	@echo "  make clean         -> limpa artefatos"
