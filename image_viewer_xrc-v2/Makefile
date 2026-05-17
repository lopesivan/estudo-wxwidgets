OS  = linux
APP = image_viewer_xrc
WX_LIB = ${HOME}/.local/wxwidgets/linux-cmake-wx-3.3.1/lib

.PHONY: all configure build clean run

all: configure build run

configure:
	cmake -B build-$(OS) -S . -DCMAKE_EXE_LINKER_FLAGS="-Wl,-rpath,$(WX_LIB)"

build:
	cmake --build build-$(OS)

clean:
	rm -rf build-$(OS)

run:
	build-$(OS)/$(APP)
