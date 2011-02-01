default: 
	g++ -lnoise $(shell pkg-config --cflags --libs OGRE OIS) ../../common/src/*.cpp *.cpp -I../../common/src/ -o ogredemo.bin

debug:
	g++ -g -lnoise $(shell pkg-config --cflags --libs OGRE OIS) ../../common/src/*.cpp *.cpp -I../../common/src/ -o ogredemo.bin

run: 
	./ogredemo.bin

