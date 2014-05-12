#ifndef LIGHT_H
#define LIGHT_H

#include "vector.h"
#include "Color.h"
#include "lightSource.h"

class Light : public LightSource 
{
public:
	Light();
	Light(Vector pos, Color color);
	~Light();

private:
};

#endif