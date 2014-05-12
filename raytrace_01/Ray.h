#ifndef RAY_H
#define RAY_H

#include "vector.h"

class Ray
{
public:
	Ray(Vector origin, Vector direction, double t = 1.0);
	~Ray();

	Vector	getDirection();
	Vector	getOrigin();

private:
	Vector	pOrigin, pDirection;
	double	pT;

};

#endif