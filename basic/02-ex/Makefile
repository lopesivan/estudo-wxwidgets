sources     = $(wildcard *.cpp)
PROGRAM     = $(sources:.cpp=)
OBJS        = $(sources:.cpp=.o)
DEPS       := $(OBJS:.o=.d)

# =========================
# Configuração do wxWidgets
# =========================
V              ?= 3.2.4
OS             ?= linux
WX_BASE_DIR    ?= $(HOME)/wx
WX_SRC_DIR     ?= $(WX_BASE_DIR)/wxWidgets-$(V)-$(OS)
WX_INSTALL_DIR ?= $(WX_BASE_DIR)/$(OS)-wx-$(V)
WX_LIB_PATH    ?= $(WX_INSTALL_DIR)/lib

# Permita sobrescrever WX_CONFIG via ambiente, senão use o padrão
WX_CONFIG      ?= $(WX_INSTALL_DIR)/bin/wx-config

# Opções do wx-config (ajuste toolkit/unicode/static se necessário)
WX_TOOLKIT     ?= gtk3
WX_UNICODE     ?= yes
WX_STATIC      ?= no
WX_VERSION     ?= 3.2
WX_CONFIG_OPT  := --unicode=$(WX_UNICODE) --static=$(WX_STATIC) --toolkit=$(WX_TOOLKIT) --version=$(WX_VERSION)

# =========================
# Toolchain e flags
# =========================
CXX            ?= $(shell $(WX_CONFIG) --cxx)
#WARNINGS      ?= -Wall -Wextra -Wpedantic
OPTIMIZE       ?= -O3
DEBUG          ?= 0        # use DEBUG=1 para build de debug
RPATH          ?= 1        # use RPATH=0 para desabilitar rpath

# Flags vindas do wx-config
#WX_CXXFLAGS   := $(shell $(WX_CONFIG) --cxxflags $(WX_CONFIG_OPT))
WX_CXXFLAGS   := $(shell $(WX_CONFIG) --cppflags $(WX_CONFIG_OPT))
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
CXXFLAGS      += $(WX_CXXFLAGS) $(DEPFLAGS) $(WARNINGS)
LDLIBS        += $(WX_LDLIBS)
LINK.o         = $(CXX) $(LDFLAGS)

all: $(PROGRAM)

$(PROGRAM): $(OBJS)

run: $(PROGRAM)
	./$(PROGRAM).exe

ldd: $(PROGRAM)
	# Ver as bibliotecas linkadas
	ldd $^| grep libwx_

readelf: $(PROGRAM)
	# Ver o RPATH definido
	readelf -d $^ | grep -E "(RPATH|RUNPATH)"

clean:
	/bin/rm -rf $(PROGRAM) $(OBJS) $(DEPS)

# Dependências automáticas
-include $(DEPS)
