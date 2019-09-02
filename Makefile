synth: build synth.cpp
	g++ -O3 -Wall -Wpedantic -lrtmidi -lrtaudio synth.cpp -o build/synth

build:
	mkdir build

.PHONY:
clean: build
	rm -rf build

.PHONY:
run: synth
	build/synth
