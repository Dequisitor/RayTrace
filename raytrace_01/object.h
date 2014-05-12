#ifndef OBJECT_H
#define OBJECT_H

#include "Ray.h"
#include "Color.h"

class Object 
{
public:
	Object(Color color) { this->pColor = color; }
	~Object() {};

	virtual Color	getShade() { return pColor; };
	virtual double	Intersection(Ray ray) { return 0.0; };
	virtual Vector	getNormalAt(Vector pos) { return Vector(); }

protected:
	Color pColor;
};

#endif