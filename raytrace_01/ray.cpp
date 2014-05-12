#include "Ray.h"

Ray::Ray(Vector origin, Vector direction, double t)
{
	this->pOrigin = origin;
	this->pDirection = direction;
	this->pT = t;
}

Ray::~Ray()
{
}

Vector Ray::getDirection()
{
	return this->pDirection;
}

Vector Ray::getOrigin()
{
	return this->pOrigin;
}