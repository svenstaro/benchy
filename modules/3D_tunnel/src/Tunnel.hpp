#ifndef ASTEROIDS_HPP_INCLUDED
#define ASTEROIDS_HPP_INCLUDED

#include "Benchmark.hpp"

class Tunnel : public Benchmark {
public:
	void setupScene();
	void stepScene();
	void run();
};

#endif