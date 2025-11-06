# Descobre todos os .cpp e define 1 binário por fonte
sources      := $(wildcard *.cpp)
PROGRAMS     := $(basename $(sources))
OBJS         := $(PROGRAMS:=.o)
DEPS         := $(OBJS:.o=.d)

# =========================
# Configuração do wxWidgets
# =========================
V              ?= 3.2.4
OS             ?= linux
WX_BASE_DIR    ?= $(HOME)/wx
WX_SRC_DIR     ?= $(WX_BASE_DIR)/wxWidgets-$(V)-$(OS)
WX_INSTALL_DIR ?= $(WX_BASE_DIR)/$(OS)-wx-$(V)
WX_LIB_PATH    ?= $(WX_INSTALL_DIR)/lib
WX_CONFIG      ?= $(WX_INSTALL_DIR)/bin/wx-config

WX_TOOLKIT     ?= gtk3
WX_UNICODE     ?= yes
WX_STATIC      ?= no
WX_VERSION     ?= 3.2
WX_CONFIG_OPT  := --unicode=$(WX_UNICODE) --static=$(WX_STATIC) --toolkit=$(WX_TOOLKIT) --version=$(WX_VERSION)

# =========================
# Toolchain e flags
# =========================
CXX            ?= $(shell $(WX_CONFIG) --cxx)
WARNINGS       ?= -Wall -Wextra -Wpedantic
OPTIMIZE       ?= -O3
DEBUG          ?= 0        # use DEBUG=1 para build de debug
RPATH          ?= 1        # use RPATH=0 para desabilitar rpath

# Flags vindas do wx-config
WX_CXXFLAGS    := $(shell $(WX_CONFIG) --cppflags $(WX_CONFIG_OPT))
WX_LDLIBS      := $(shell $(WX_CONFIG) $(WX_CONFIG_OPT) --libs core,base)

# Dep (-MMD -MP) para gerar .d e recompilar só o necessário
DEPFLAGS       := -MMD -MP

# Ajuste de flags por modo
ifeq ($(DEBUG),1)
  CXXFLAGS     ?= -O0 -g
  CPPFLAGS     ?=
else
  CXXFLAGS     ?= $(OPTIMIZE)
  CPPFLAGS     ?=
endif

# rpath opcional apontando para as libs do wx instaladas localmente
ifeq ($(RPATH),1)
  LDFLAGS      ?= -Wl,-rpath,$(WX_LIB_PATH)
else
  LDFLAGS      ?=
endif

# Compose final
CXXFLAGS      += $(WX_CXXFLAGS) $(DEPFLAGS) $(WARNINGS)
LDLIBS        += $(WX_LDLIBS)
LINK.o         = $(CXX) $(LDFLAGS)

# =========================
# Alvos
# =========================
.PHONY: all clean run ldd readelf
.SUFFIXES:

all: $(PROGRAMS)

# Regra para compilar cada .cpp em seu .o
%.o: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

# Regra para linkar cada programa a partir do seu .o
%: %.o
	$(LINK.o) $< $(LDLIBS) -o $@

# Helpers por programa: make run-foo, make ldd-foo, make readelf-foo
run-%: %
	./$<

ldd-%: %
	ldd $< | grep libwx_ || true

readelf-%: %
	readelf -d $< | grep -E "(RPATH|RUNPATH)" || true

# Versões que rodam em todos os binários
run: $(PROGRAMS)
	@for p in $^; do echo "== $$p =="; ./$$p || exit $$?; echo; done

ldd: $(PROGRAMS)
	@for p in $^; do echo "== $$p =="; ldd $$p | grep libwx_ || true; echo; done

readelf: $(PROGRAMS)
	@for p in $^; do echo "== $$p =="; readelf -d $$p | grep -E "(RPATH|RUNPATH)" || true; echo; done

clean:
	$(RM) $(PROGRAMS) $(OBJS) $(DEPS)

# Dependências automáticas
-include $(DEPS)
