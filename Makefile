synth: build main.cpp synth.h synth.cpp
	g++ -O3 -Wall -Wpedantic -lrtmidi -lrtaudio main.cpp synth.cpp -o build/synth

build:
	mkdir build

.PHONY:
clean: build
	rm -rf build

.PHONY:
run: synth
	build/synth
