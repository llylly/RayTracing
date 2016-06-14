#ifndef COLOR_H
#define COLOR_H

class Color {
public:
	double R, G, B;
	Color() {
		R = G = B = 0.0f;
	}
	Color(const Color& c) {
		R = c.R, G = c.G, B = c.B;
	}
	Color(double r, double g, double b) {
		R = r, G = g, B = b;
	}
	void set(double r, double g, double b) {
		R = r, G = g, B = b;
	}
	Color &operator+=(const Color&);
	Color &operator*=(double);
	Color &operator*=(const Color&);
	Color &operator/=(double);
};

Color operator*(double, const Color&);
Color operator*(const Color&, double);
Color operator*(const Color&, const Color&);
Color operator+(const Color&, const Color&);

Color operator/(const Color&, double);
Color operator/(const Color&, const Color&);

#endif
