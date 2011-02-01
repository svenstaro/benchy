#ifndef ASTEROIDS_HPP_INCLUDED
#define ASTEROIDS_HPP_INCLUDED

#include "Benchmark.hpp"
#include <noise/noise.h>

class Tunnel : public Benchmark {
public:
	void setupScene();
	void stepScene();
	void run();

private:
	Ogre::Vector3 GetDisplacement(float z);
};

#endif
