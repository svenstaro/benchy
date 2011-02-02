#ifndef ASTEROIDS_HPP_INCLUDED
#define ASTEROIDS_HPP_INCLUDED

#include "Benchmark.hpp"
#include <noise/noise.h>

class Tunnel : public Benchmark {
public:
	Tunnel(float radius = 30, 
			float segments = 24, 
			float parts = 100, 
			float part_length = 10,
			float speed = 200);
	void setupScene();
	void stepScene();
	void run();

	void GenerateTunnel(int first_part = 0);
	void AddQuad(Ogre::ManualObject* o, float alpha, int part);
private:
	Ogre::Vector3 GetDisplacement(float z);
	Ogre::ManualObject* mTunnelObject;
	float mRadius;
	float mSegments;
	float mParts;
	float mPartLength;
	float mSpeed;
};

#endif
