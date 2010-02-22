default: 
	g++ $(shell pkg-config --cflags --libs OGRE OIS) ../../common/src/*.cpp *.cpp -I../../common/src/ -o ogredemo

debug:
	g++ -g $(shell pkg-config --cflags --libs OGRE OIS) ../../common/src/*.cpp *.cpp -I../../common/src/ -o ogredemo

run: default
	./ogredemo

