#include "Color.h"

Color &Color::operator+=(const Color &b) {
	R += b.R, G += b.G, B += b.B;
	return *this;
}

Color &Color::operator*=(double fac) {
	R*=fac, G*=fac, B*=fac;
	return *this;
}

Color &Color::operator*=(const Color &c) {
	R*=c.R, G*=c.G, B*=c.B;
	return *this;
}

Color &Color::operator/=(double fac) {
	R/=fac, G/=fac, B/=fac;
	return *this;
}

Color operator*(double fac, const Color &b) {
	Color ans = b;
	ans.R*=fac, ans.G*=fac, ans.B*=fac;
	return ans;
}

Color operator*(const Color &b, double fac) {
	Color ans = b;
	ans.R*=fac, ans.G*=fac, ans.B*=fac;
	return ans;
}

Color operator*(const Color &a, const Color &b) {
	return Color(a.R * b.R, a.G * b.G, a.B * b.B);
}

Color operator+(const Color &a, const Color &b) {
	return Color(a.R + b.R, a.G + b.G, a.B + b.B);
}

Color operator/(const Color &b, double fac) {
	Color ans = b;
	ans.R/=fac, ans.G/=fac, ans.B/=fac;
	return ans;
}