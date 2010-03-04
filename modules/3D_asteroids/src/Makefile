default: 
	g++ $(shell pkg-config --cflags --libs OGRE OIS) ../../common/src/*.cpp *.cpp -I../../common/src/ -o ogredemo.bin

debug:
	g++ -g $(shell pkg-config --cflags --libs OGRE OIS) ../../common/src/*.cpp *.cpp -I../../common/src/ -o ogredemo.bin

run: default
	./ogredemo.bin

