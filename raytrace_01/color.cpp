#include "Color.h"

Color::Color()
{
	this->pRed		= 1.0;
	this->pGreen	= 1.0;
	this->pBlue		= 1.0;
	this->pSpec		= 0.0;
}

Color::Color(double r, double g, double b, double spec)
{
	this->pRed		= r;
	this->pGreen	= g;
	this->pBlue		= b;
	this->pSpec		= spec;
}

void Color::setRed(double red)
{
	this->pRed = red;
}

void Color::setGreen(double green)
{
	this->pGreen = green;
}

void Color::setBlue(double blue)
{
	this->pBlue = blue;
}

void Color::setSpec(double spec)
{
	this->pSpec = spec;
}

double Color::red()
{
	return this->pRed;
}

double Color::green()
{
	return this->pGreen;
}

double Color::blue()
{
	return this->pBlue;
}

double Color::special()
{
	return this->pSpec;
}

Color& Color::operator=(Color param)
{
	this->pRed		= param.red();
	this->pGreen	= param.green();
	this->pBlue		= param.blue();
	this->pSpec		= param.special();
	
	return *this;
}

Color operator+(Color a, Color b)
{
	return Color(a.red() + b.red(), a.green() + b.green(), a.blue() + b.blue());
}

Color operator*(Color a, Color b)
{
	return Color(a.red() * b.red(), a.green() * b.green(), a.blue() * b.blue());
}

Color::~Color()
{
}

double Color::getBrightness()
{
	return pRed + pBlue + pGreen;
}

Color Color::Brighten(double scalar)
{
	return Color(pRed * scalar, pGreen * scalar, pBlue * scalar);
}

Color	AddColor(Color a, Color b)
{
	return Color(a.red()+b.red(), a.green()+b.green(), a.blue()+b.blue());
}

Color	MultiplyColor(Color a, Color b)
{
	return Color(a.red()*b.red(), a.green()*b.green(), a.blue()*b.blue());
}

Color	AverageColor(Color a, Color b)
{
	return Color((a.red() + b.red())/2.0, (a.green() + b.green())/2.0, (a.blue() + b.blue())/2.0);
}

Color	Color::clip()
{
	double r = pRed, g = pGreen, b = pBlue;
	double allLight = r + b + g;
	double excessLight = allLight - 3.0;
	if (excessLight > 0) {
		r += excessLight*(r/allLight);
		g += excessLight*(g/allLight);
		b += excessLight*(b/allLight);
	}

	if (r > 1.0)
	{
		r = 1.0;
	}
	if (g > 1.0)
	{
		g = 1.0;
	}
	if (b > 1.0)
	{
		b = 1.0;
	}

	if (r < 0.0)
	{
		r = 0.0;
	}
	if (g < 0.0)
	{
		g = 0.0;
	}
	if (b < 0.0)
	{
		b = 0.0;
	}

	return Color(r, g, b, pSpec);
}