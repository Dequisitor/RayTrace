#include "light.h"

Light::Light() 
	: LightSource(Vector(), Color())
{
}

Light::Light(Vector pos, Color color)
	: LightSource(pos, color)
{
}

Light::~Light()
{
}
