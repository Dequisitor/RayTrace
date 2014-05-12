#include "vector.h"
#include <cmath>

Vector::Vector(double x, double y, double z, double w)
{
	this->pX = x;
	this->pY = y;
	this->pZ = z;
	this->pW = w;
}

Vector::Vector()
{
	this->pX = 0.0;
	this->pY = 0.0;
	this->pZ = 0.0;
	this->pW = 0.0;
}

Vector::~Vector()
{
}

Vector& Vector::operator=(Vector param)
{
	this->pX = param.x();
	this->pY = param.y();
	this->pZ = param.z();
	this->pW = param.w();
	
	return *this;
}

Vector operator+(Vector a, Vector b)
{
	return Vector(a.x() + b.x(), a.y() + b.y(), a.z() + b.z());
}

Vector operator*(Vector a, double b)
{
	return Vector(a.x() * b, a.y() * b, a.z() * b);
}

double Vector::x() 
{
	return this->pX;
}

double Vector::y()
{
	return this->pY;
}

double Vector::z()
{
	return this->pZ;
}

double Vector::w()
{
	return this->pW;
}

Vector Vector::Normalize()
{
	double magnitude = sqrt(pX*pX + pY*pY + pZ*pZ);
	return Vector(pX/magnitude, pY/magnitude, pZ/magnitude);
}

double Vector::Magnitude()
{
	return sqrt(pX*pX + pY*pY + pZ*pZ);
}

Vector Vector::Invert()
{
	return Vector(-pX, -pY, -pZ);
}

double dotProduct(Vector a, Vector b)
{
	return a.x()*b.x() + a.y()*b.y() + a.z()*b.z();
}

Vector crossProduct(Vector a, Vector b)
{
	return Vector(a.y()*b.z() - a.z()*b.y(), a.z()*b.x() - a.x()*b.z(), a.x()*b.y() - a.y()*b.x());
}

Vector AddVectors(Vector a, Vector b)
{
	return Vector(a.x() + b.x(), a.y() + b.y(), a.z() + b.z());
}

Vector MulVector(Vector a, double b)
{
	return Vector(a.x() * b, a.y() * b, a.z() * b);
}