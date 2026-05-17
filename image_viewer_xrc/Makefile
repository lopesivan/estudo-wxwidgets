.PHONY: configure build clean run

configure:
	cmake -B build-linux -S .

build:
	cmake --build build-linux
	make -C build-linux copy_dlls

deploy: configure build run

clean:
	rm -rf build-linux

run:
	linuxe build-win/image_viewer.exe

