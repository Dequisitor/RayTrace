#include "Plane.h"

Plane::Plane(Vector norm, double dist, Color color)
	: Object(color)
{
	pNormal = norm;
	pDistance = dist;
}

Plane::~Plane()
{
}

Vector Plane::getNormal()
{
	return pNormal;
}

double Plane::getDistance()
{
	return pDistance;
}

double Plane::Intersection(Ray ray)
{
	Vector rayDirection = ray.getDirection();
	double cosine = dotProduct(rayDirection, pNormal);

	if (cosine == 0)
	{
		//ray is parallel to plane
		return -1.0;
	}
	else
	{
		double b = dotProduct(pNormal, (ray.getOrigin() + (pNormal * pDistance).Invert()));
		return -1*b/cosine;
	}
}

Vector Plane::getNormalAt(Vector pos)
{
	return pNormal;
}