#ifndef LIGHT_SOURCE_H
#define LIGHT_SOURCE_H

#include "vector.h"
#include "Color.h"

class LightSource {
public:
	LightSource(Vector pos, Color color)
	{
		this->pPosition = pos;
		this->pColor = color;
	}
	
	virtual Vector	getPosition() { return pPosition; };
	virtual Color	getColor() { return pColor; };

private:
	Vector	pPosition;
	Color	pColor;
};

#endif