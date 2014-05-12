#ifndef COLOR_H
#define COLOR_H

class Color {
public:
	Color();
	Color(double r, double g, double b, double spec = 0.0);
	~Color();

	Color&			operator=(Color param);
	friend Color	operator+(Color a, Color b);
	friend Color	operator*(Color a, Color b);

	double	red();
	double	green();
	double	blue();
	double	special();

	double	getBrightness();
	Color	Brighten(double scalar);
	Color	clip();

	void	setRed(double red);
	void	setGreen(double green);
	void	setBlue(double blue);
	void	setSpec(double spec);

private:
	double pRed, pGreen, pBlue, pSpec;

};

Color	AddColor(Color a, Color b);
Color	MultiplyColor(Color a, Color b);	
Color	AverageColor(Color a, Color b);

#endif