synth: build *.h *.cpp
	g++ -O3 -lrtmidi -lrtaudio *.cpp -o build/synth

build:
	mkdir build

.PHONY:
clean: build
	rm -rf build

.PHONY:
run: synth
	build/synth
