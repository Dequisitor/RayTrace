#include "viewPoint.h"

ViewPoint::ViewPoint(Vector position, Vector direction, Vector vpRight, Vector vpDown)
{
	this->pPosition = position;
	this->pDirection = direction;
	this->pRight = vpRight;
	this->pDown = vpDown;
}

ViewPoint::~ViewPoint()
{
}

Vector ViewPoint::getDirection()
{
	return pDirection;
}

Vector ViewPoint::getPosition()
{
	return pPosition;
}

Vector ViewPoint::getRight()
{
	return pRight;
}

Vector ViewPoint::getDown()
{
	return pDown;
}