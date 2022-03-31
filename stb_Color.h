/*
	STB-Like Color Utils
	Author: Krivenko Slava (NeoWise69/thekrovee)
	
	v 0.01 03.31.2022 - Initial minimal set of functions. WIP.
*/
#ifndef _STB_COLOR_
#define _STB_COLOR_

/*
* Simple Color class for manipulating with color values.
*/
class Color {
public:
	float R, G, B, A;

	Color() = default;
	Color(const Color&) = default;
	Color(float r, float g, float b);
	Color(float r, float g, float b, float a);

	unsigned int ToUInt32() const;

	Color operator+(const Color& c) const;
	Color operator-(const Color& c) const;
	Color operator*(const Color& c) const;
	Color operator/(const Color& c) const;

};

static Color const Red =    {1.0f, 0.0f, 0.0f, 1.0f};
static Color const Green =  {0.0f, 1.0f, 0.0f, 1.0f};
static Color const Blue =   {0.0f, 0.0f, 1.0f, 1.0f};
static Color const Pink =   {1.0f, 0.0f, 1.0f, 1.0f};
static Color const Black =  {0.0f, 0.0f, 0.0f, 1.0f};
static Color const White =  {1.0f, 1.0f, 1.0f, 1.0f};

#ifdef STB_COLOR_IMPLEMENTATION

Color::Color(float r, float g, float b) : R(r), G(g), B(b), A(1.0f) {}

Color::Color(float r, float g, float b, float a) : R(r), G(g), B(b), A(a) {}

unsigned int Color::ToUInt32() const {
	unsigned int r = (unsigned int)(R * 255.0f);
	unsigned int g = (unsigned int)(G * 255.0f);
	unsigned int b = (unsigned int)(B * 255.0f);
	unsigned int a = (unsigned int)(A * 255.0f);

	return (unsigned int)((a << 24) | (r << 16) | (g << 8) | b);
}

inline Color Color::operator+(const Color& c) const {
	return Color(R + c.R, G + c.G, B + c.B, A + c.A);
}

inline Color Color::operator-(const Color& c) const {
	return Color(R - c.R, G - c.G, B - c.B, A - c.A);
}

inline Color Color::operator*(const Color& c) const {
	return Color(R * c.R, G * c.G, B * c.B, A * c.A);
}

inline Color Color::operator/(const Color& c) const {
	return Color(R / c.R, G / c.G, B / c.B, A / c.A);
}

#endif // !ifdef STB_COLOR_IMPLEMENTATION

#endif // !ifndef _STB_COLOR_
