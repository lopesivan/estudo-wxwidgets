OS  = linux
APP = image_viewer_xrc
WX_LIB = ${HOME}/.local/wxwidgets/linux-cmake-wx-3.3.1/lib

.PHONY: all configure build clean run

all: configure build run

configure:
	cmake -B build-$(OS) -S .

build:
	cmake --build build-$(OS)

clean:
	rm -rf build-$(OS)

run:
	LD_LIBRARY_PATH="$(WX_LIB):$$LD_LIBRARY_PATH" build-$(OS)/$(APP)
