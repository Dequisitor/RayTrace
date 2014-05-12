#include "Sphere.h"
#include <cmath>

Sphere::Sphere(Vector center, double radius, Color color)
	: Object(color)
{
	this->pCenter = center;
	this->pRadius = radius;
}

Sphere::~Sphere()
{

}

Vector Sphere::getCenter()
{
	return pCenter;
}

double Sphere::getRadius()
{
	return pRadius;
}

Vector Sphere::getNormalAt(Vector pos)
{
	Vector normal = (pos + pCenter.Invert()).Normalize();
	return normal;
}

double Sphere::Intersection(Ray ray)
{
	double a = 1.0;
	double b = (2.0 * (ray.getOrigin().x() - pCenter.x()) * ray.getDirection().x()) +
		(2.0 * (ray.getOrigin().y() - pCenter.y()) * ray.getDirection().y()) +
		(2.0 * (ray.getOrigin().z() - pCenter.z()) * ray.getDirection().z());
	double c = pow(ray.getOrigin().x() - pCenter.x(), 2) + pow(ray.getOrigin().y() - pCenter.y(), 2) + pow(ray.getOrigin().z() - pCenter.z(), 2) - pow(pRadius, 2);;

	double discriminant = b*b - 4*a*c;

	if (discriminant > 0.0) 
	{
		double firstRoot = ((-b - sqrt(discriminant))/2.0*a) - 0.0000000001;
		double secondRoot = ((-b + sqrt(discriminant))/2.0*a) - 0.0000000001;
		
		return firstRoot > 0 ? firstRoot : secondRoot;
	}
	else
	{
		return -1.0;
	}
}