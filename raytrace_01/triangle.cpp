#include "triangle.h"

Triangle::Triangle(Vector a, Vector b, Vector c, Color color)
	: Object(color)
{
	this->A = a;
	this->B = b;
	this->C = c;

	this->pNormal = CalculateNormal();
	this->pDistance = CalculateDistance();
}

Vector Triangle::getNormal()
{
	return pNormal;
}

Vector Triangle::getNormalAt(Vector pos)
{
	return pNormal;
}

double Triangle::getDistance()
{
	return pDistance;
}

Vector Triangle::CalculateNormal()
{
	Vector CA(C.x() - A.x(), C.y() - A.y(), C.z() - A.z());
	Vector BA(B.x() - A.x(), B.y() - A.y(), B.z() - A.z());

	Vector normal = crossProduct(CA, BA).Normalize();

	return normal;
}

double Triangle::CalculateDistance()
{
	return dotProduct(pNormal, A);
}

double Triangle::Intersection(Ray ray)
{
	double cosine = dotProduct(ray.getDirection(), pNormal);

	if (cosine == 0.0)
	{
		return -1.0;
	}
	else
	{
		double b = dotProduct(pNormal, (ray.getOrigin() + (pNormal * pDistance).Invert()));
		double distance = -1*b/cosine;

		//intersection point with plane
		Vector Q = ray.getDirection() * distance + ray.getOrigin();
		Vector QA = Q + A.Invert();
		Vector QB = Q + B.Invert();
		Vector QC = Q + C.Invert();
		Vector CA = C + A.Invert();
		Vector BC = B + C.Invert();
		Vector AB = A + B.Invert();

		double testA = dotProduct(crossProduct(CA, QA), pNormal);
		double testB = dotProduct(crossProduct(AB, QB), pNormal);
		double testC = dotProduct(crossProduct(BC, QC), pNormal);

		if (testA >= 0 && testB >= 0 && testC >= 0) 
		{
			return distance;
		}
		else
		{
			return -1.0;
		}
	}
}