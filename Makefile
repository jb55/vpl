
all: build/vpl-ide

build/vpl-ide: build/Makefile
	cd build && make

build/Makefile: premake4.lua
	premake4 gmake

clean:
	rm -rf build

.PHONY: clean
