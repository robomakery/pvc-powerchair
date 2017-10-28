BOARD_TAG = uno
# MONITOR_PORT  = /dev/ttyACM0
ARDUINO_LIBS = U8g2 SPI Wire SabertoothSimplified

# why doesn't just Arduino.mk work?
include /usr/share/arduino/Arduino.mk

SHELL := /bin/bash

run-tests:
	rm -rf test/build
	mkdir test/build
	g++ -c test/test_main.cpp -o test/build/test_main.o
	g++ -c test/test_factorial.cpp -o test/build/test_factorial.o
	g++ test/build/test_main.o test/build/test_factorial.o -o test/build/catch
	./test/build/catch
