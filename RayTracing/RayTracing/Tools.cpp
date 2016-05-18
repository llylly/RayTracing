#include "Tools.h"

double powF(double x, int a) {
	double ans = 1.0f;
	while (a) {
		if (a&1) ans *= x;
		a>>=1, x*=x;
	}
	return ans;
}

string intToString(int x) {
	string s = "", tmp = "0";
	bool negative = false;
	if (x == 0) return "0";
	if (x < 0) negative = true, x = -x;
	while (x) {
		tmp[0] = x % 10 + 48;
		s.insert(0, tmp);
		x /= 10;
	}
	if (negative)
		s = "-" + s;
	return s;
}