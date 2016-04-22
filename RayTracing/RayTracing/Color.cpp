#include "Color.h"

Color &Color::operator+=(const Color &b) {
	R += b.R, G += b.G, B += b.B;
	R = (R>=1.0f)?1.0f: R;
	G = (G>=1.0f)?1.0f: G;
	B = (B>=1.0f)?1.0f: B;
	return *this;
}

Color &Color::operator*=(double fac) {
	R*=fac, G*=fac, B*=fac;
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
