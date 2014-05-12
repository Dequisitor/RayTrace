#ifndef VECTOR_H
#define VECTOR_H

class Vector
{
public:
	Vector();
	Vector(double x, double y, double z, double w = 0.0);
	~Vector();

	double	x();
	double	y();
	double	z();
	double	w();
	
	Vector&	operator=(Vector param);
	friend Vector	operator+(Vector a, Vector b);
	friend Vector	operator*(Vector a, double b);
	Vector	Normalize();
	Vector	Invert();
	double	Magnitude();
		
private:
	double pX, pY, pZ, pW;

};

double dotProduct(Vector a, Vector b);
Vector crossProduct(Vector a, Vector b);
Vector AddVectors(Vector a, Vector b);
Vector MulVector(Vector a, double b);

#endif