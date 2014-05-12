#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "vector.h"
#include "object.h"

class Triangle : public Object
{
public:
	Triangle(Vector a, Vector b, Vector c, Color color);

	virtual double Intersection(Ray ray);
	virtual Vector getNormalAt(Vector pos);
	Vector	getNormal();
	double	getDistance();

private:
	Vector	A;
	Vector	B;
	Vector	C;
	Vector	pNormal;
	double	pDistance;

	Vector	CalculateNormal();
	double	CalculateDistance();
};

#endif