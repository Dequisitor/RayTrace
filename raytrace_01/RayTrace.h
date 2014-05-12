#ifndef RAYTRACE_H
#define RAYTRACE_H

#include <vector>
#include "Sphere.h"
#include "object.h"
#include "viewPoint.h"
#include "Light.h"
#include "Plane.h"
#include "Timer.h"
#include "triangle.h"
#include "UI.h"

class RayTrace
{
public:
	static RayTrace& getInstance();
	
	void	Render();
	void	Draw(HWND hwnd);
	bool	isReady();

private:
	RayTrace();
	~RayTrace();

	int		getForemost(std::vector<double> intersectionList);
	Color	getColorAt(Vector intPosition, Vector intDirection, std::vector<Object*> objects, int foremostObject, std::vector<LightSource*> lights, double accuracy, double ambientLight);

	unsigned char* 	screen;
	bool			ready;

	//constants
	static const unsigned int	height;
	static const unsigned int	width;
	static const double			aspectRatio;
	static const double			accuracy;

};

#endif