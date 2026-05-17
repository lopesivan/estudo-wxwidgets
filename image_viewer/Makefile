.PHONY: configure build clean run

configure:
	cmake -B build-win -S . -DCMAKE_TOOLCHAIN_FILE=toolchains/mingw64.cmake

build:
	cmake --build build-win
	make -C build-win copy_dlls

deploy: configure build run

clean:
	rm -rf build-win

run:
	wine build-win/image_viewer.exe

