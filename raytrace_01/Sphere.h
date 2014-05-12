#ifndef SPHERE_H
#define SPHERE_H

#include "object.h"

class Sphere : public Object
{
public:
	Sphere(Vector center, double radius, Color color);
	~Sphere();

	Vector	getCenter();
	double	getRadius();
	//Color	getShade();

	virtual Vector	getNormalAt(Vector pos);
	virtual double	Intersection(Ray ray);
private:
	Vector	pCenter;
	double	pRadius;

};

#endif
