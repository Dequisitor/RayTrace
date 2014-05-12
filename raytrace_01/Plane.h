#ifndef PLANE_H
#define PLANE_H

#include "object.h"

class Plane : public Object 
{
public:
	Plane(Vector norm, double dist, Color color);
	~Plane();

	Vector	getNormal();
	double	getDistance();

	virtual Vector	getNormalAt(Vector pos);
	virtual double	Intersection(Ray ray);
private:
	Vector	pNormal;
	double	pDistance;
	//Color	pColor;
};

#endif