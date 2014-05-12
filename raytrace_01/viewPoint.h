#ifndef VIEWPOINT_H
#define VIEWPOINT_H

#include "vector.h"

class ViewPoint 
{
public:
	ViewPoint(Vector position, Vector direction, Vector vpRight, Vector vpDown);
	~ViewPoint();

	Vector	getPosition();
	Vector	getDirection();
	Vector	getRight();
	Vector	getDown();

private:
	Vector	pPosition;
	Vector	pDirection;
	Vector	pRight;
	Vector	pDown;

	double	pWidth;
	double	pHeight;
	double	pFoV;
};

#endif