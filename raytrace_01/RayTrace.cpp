#include "RayTrace.h"

const unsigned int	RayTrace::height		= 480;
const unsigned int	RayTrace::width			= 640;
const double		RayTrace::aspectRatio	= (double)width/(double)height;
const double		RayTrace::accuracy		= 0.00000001;

RayTrace& RayTrace::getInstance()
{
	static RayTrace instance;
	return instance;
}

RayTrace::RayTrace()
{
	ready = false;
	screen = new unsigned char[width*height*4];
	memset(screen, 128, width*height*4);
}

RayTrace::~RayTrace()
{
	delete screen;
}

void RayTrace::Draw(HWND hwnd)
{	
	int error;
	PAINTSTRUCT ps = {0};
	HDC hdc = GetDC(hwnd);
	error = GetLastError();
	HDC hdcmem = CreateCompatibleDC(hdc);
	error = GetLastError();
	HBITMAP bitmap = CreateCompatibleBitmap(hdc, width, height);
	error = GetLastError();
	int count = SetBitmapBits(bitmap, width*height*4, screen);
	error = GetLastError();
	SelectObject(hdcmem, bitmap);
	error = GetLastError();

	BitBlt(hdc, 0, 0, width, height, hdcmem, 0, 0, SRCCOPY);
	error = GetLastError();

	DeleteDC(hdcmem);
	error = GetLastError();
	DeleteObject(bitmap);
	error = GetLastError();
	DeleteDC(hdc);
	error = GetLastError();
}

int RayTrace::getForemost(std::vector<double> intersectionList)
{
	if (intersectionList.size()  == 0)
	{
		return -1;
	}
	else if (intersectionList.size() == 1) 
	{
		if (intersectionList.at(0) > 0)
		{
			return 0;
		}
		else 
		{
			return -1;
		}		
	}
	else 
	{
		int maxIndex = 0;
		for (unsigned int i = 1; i < intersectionList.size(); i++)
		{
			if (intersectionList.at(i) > intersectionList.at(maxIndex))
			{
				maxIndex = i;
			}
		}

		if (intersectionList.at(maxIndex) > 0.0) 
		{
			for (unsigned int i = 0; i < intersectionList.size(); i++)
			{
				if (intersectionList.at(i) > 0.0 && intersectionList.at(i) < intersectionList.at(maxIndex))
				{
					maxIndex = i;
				}
			}
			return maxIndex;
		}
		else 
		{
			return -1;
		}
	}
}

Color RayTrace::getColorAt(Vector intPosition, Vector intDirection, std::vector<Object*> objects, int foremostObject, std::vector<LightSource*> lights, double accuracy, double ambientLight)
{
	Color	intColor = objects.at(foremostObject)->getShade();
	Vector	intNormal = objects.at(foremostObject)->getNormalAt(intPosition);

	if (intColor.special() == 2)
	{
		int square = (int)floor(intPosition.x()) + (int)floor(intPosition.z());

		if ((square % 2) == 0)
		{
			intColor.setRed(0.0);
			intColor.setGreen(0.0);
			intColor.setBlue(0.0);
		}
		else
		{
			intColor.setRed(1.0);
			intColor.setGreen(1.0);
			intColor.setBlue(1.0);
		}
	}

	Color	finalColor = intColor.Brighten(ambientLight);

	if (intColor.special() > 0.0 && intColor.special() <= 1.0)
	{
		double dot1 = dotProduct(intNormal, intDirection.Invert());
		Vector scalar1 = intNormal * dot1;
		Vector add1 = scalar1 + intDirection;
		Vector scalar2 = add1 * 2.0;
		Vector add2 = intDirection.Invert() + scalar2;
		Vector reflectionDirection = add2.Normalize();

		Ray reflectionRay(intPosition, reflectionDirection);

		std::vector<double> reflectionIntersections;
		for (unsigned int objIndex = 0; objIndex < objects.size(); objIndex++)
		{
			reflectionIntersections.push_back(objects.at(objIndex)->Intersection(reflectionRay));
		}

		int foremostReflection = getForemost(reflectionIntersections);

		if (foremostReflection != -1.0)
		{
			if (reflectionIntersections.at(foremostReflection) > accuracy)
			{
				Vector reflectionPosition = intPosition + (reflectionDirection * reflectionIntersections.at(foremostReflection));

				Color reflectionColor = getColorAt(reflectionPosition, reflectionDirection, objects, foremostReflection, lights, accuracy, ambientLight);

				finalColor = finalColor + reflectionColor.Brighten(intColor.special());
			}
		}
	}

	for (unsigned int lightIndex = 0; lightIndex < lights.size(); lightIndex++)
	{
		Vector lightDirection = (lights.at(lightIndex)->getPosition() + intPosition.Invert()).Normalize();

		double cosine_angle = dotProduct(intNormal, lightDirection);

		if (cosine_angle > 0.0)
		{
			bool shadowed = false;

			Vector lightDistance = (lights.at(lightIndex)->getPosition() + intPosition.Invert());
			double lightDistanceMagnitude = lightDistance.Magnitude();
			lightDistance = lightDistance.Normalize();

			Ray shadowRay(intPosition, (lights.at(lightIndex)->getPosition() + intPosition.Invert()).Normalize());

			std::vector<double> secondaryIntersections;
			for (unsigned int objIndex = 0; objIndex < objects.size() && shadowed == false; objIndex++)
			{
				secondaryIntersections.push_back(objects.at(objIndex)->Intersection(shadowRay));
			}

			for (unsigned int i = 0; i < secondaryIntersections.size(); i++)
			{
				if (secondaryIntersections.at(i) > accuracy)
				{
					if (secondaryIntersections.at(i) <= lightDistanceMagnitude)
					{
						shadowed = true;
					}
					break;
				}
			}

			if (shadowed == false) {
				finalColor = finalColor + ((intColor * lights.at(lightIndex)->getColor()).Brighten(cosine_angle));

				if (intColor.special() > 0.0 && intColor.special() <= 1.0) //shiny
				{
					double dot1 = dotProduct(intNormal, intDirection.Invert());
					Vector scalar1 = intNormal * dot1;
					Vector add1 = scalar1 + intDirection;
					Vector scalar2 = add1 * 2.0;
					Vector add2 = intDirection.Invert() + scalar2;
					Vector reflectionDirection = add2.Normalize();

					double specular = dotProduct(reflectionDirection, lightDirection);
					if (specular > 0.0)
					{
						specular = pow(specular, 10);
						finalColor = finalColor + lights.at(lightIndex)->getColor().Brighten(specular * intColor.special());
					}
				}
			}

		}
	}

	return finalColor.clip();
}

void RayTrace::Render()
{
	Vector	X(1.0, 0.0, 0.0);
	Vector	Y(0.0, 1.0, 0.0);
	Vector	Z(0.0, 0.0, 1.0);

	Vector	camPos(-4.0, 2.0, -4.0);
	Vector	lookAt(0.0, 0.0, 0.0);
	Vector	lookDir(camPos.x() - lookAt.x(), camPos.y() - lookAt.y(), camPos.z() - lookAt.z());
	Vector	camDir(lookDir.Invert().Normalize());
	Vector	camRight = crossProduct(Y, camDir).Normalize();
	Vector	camDown = crossProduct(camRight, camDir);

	ViewPoint camera(camPos, camDir, camRight, camDown);

	//colors
	Color	whiteLight(1.0, 1.0, 1.0, 0.0);
	Color	white(1.0, 0.0, 0.0, 0.1);
	Color	black(0.0, 0.0, 0.0, 0.0);
	Color	tile(0.0, 0.0, 0.0, 2.0);
	Color	green(0.5, 1.0, 0.5, 0.5);
	Color	maroon(0.5, 0.25, 0.25, 0.5);
	Color	orange(0.94, 0.75, 0.31, 0.0);
	Color	yellow(0.9, 0.9, 0.31, 0.8);
	Color	blue(0.2, 0.2, 1.0, 0.0);

	//lights
	Vector	lightPos(-12.0, 10.0, 5.0);
	Light	sceneLight(lightPos, whiteLight);
	std::vector<LightSource*> lights;
	lights.push_back(dynamic_cast<LightSource*>(&sceneLight));

	//create objects
	Sphere	mainSphere(Vector(1.1, 1.1, 1.1), 1.0, green);
	Sphere	secondSphere(Vector(0.8, 2.0, -1.5), 0.5, white);
	Plane	mainPlane(Y, -1.0, tile);
	std::vector<Object*> objects;
	objects.push_back(dynamic_cast<Object*>(&mainSphere));
	objects.push_back(dynamic_cast<Object*>(&secondSphere));
	objects.push_back(dynamic_cast<Object*>(&mainPlane));
	
	double	ambientLight = 0.2;
	int		currentPos = 0;
	double	xamnt, yamnt;

	int aadepth = 1;
	for (unsigned int y = 0; y < height; y++)
	{
		for (unsigned int x = 0; x < width; x++)
		{
			currentPos = y * width + x;
	
			if (width > height)
			{
				xamnt = ((double)x+0.5)/(double)width * aspectRatio - (((width-height)/(double)height)/2.0);
				yamnt = (y + 0.5)/(double)height;
			}
			else if (height > width) 
			{
				xamnt = (x + 0.5) / (double)width;
				yamnt = ((y + 0.5)/(double)height) / aspectRatio - (((height - width)/(double)width)/2.0);
			}
			else
			{
				xamnt = (x + 0.5) / (double)width;
				yamnt = (y + 0.5) / (double)height;
			}

			Vector	camRayDirection = (camDir + (camRight * (xamnt - 0.5)) + (camDown * (yamnt -0.5))).Normalize();
			Ray		mainRay(camPos, camRayDirection);

			std::vector<double> intersections;

			for (unsigned int index = 0; index < objects.size(); index++)
			{
				intersections.push_back(objects.at(index)->Intersection(mainRay));
			}

			int foremostObject = getForemost(intersections);

			if (foremostObject >=0)
			{
				if (intersections.at(foremostObject) > accuracy) {

					Vector intersectionPosition = mainRay.getOrigin() + (mainRay.getDirection() * intersections.at(foremostObject));

					Color iColor = getColorAt(intersectionPosition, mainRay.getDirection(), objects, foremostObject, lights, accuracy, ambientLight);

					screen[currentPos*4 +2] = (unsigned char)(iColor.red() * 255.0);
					screen[currentPos*4 +1] = (unsigned char)(iColor.green() * 255.0);
					screen[currentPos*4 +0] = (unsigned char)(iColor.blue() * 255.0);			
					screen[currentPos*4 +3] = 0;
				}
			}
			else
			{
				screen[currentPos*4 +2] = 0;
				screen[currentPos*4 +1] = 0;
				screen[currentPos*4 +0] = 0;
				screen[currentPos*4 +3] = 0;
			}


		} //y
	} //x 

	ready = true;
}

bool RayTrace::isReady()
{
	return ready;
}