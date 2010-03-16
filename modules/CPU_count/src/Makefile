default: 
	g++ -Wall -O2 -lboost_thread-mt -lboost_system-mt *.cpp -ocount.bin

debug:
	g++ -Wall -g -lboost_thread-mt -lboost_system-mt *.cpp -o count.bin

run: default
	./count.bin

