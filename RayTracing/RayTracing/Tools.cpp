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

Vector triEquationSolver(const Vector& a, const Vector &b, const Vector &c, const Vector &d) {
	Vector ans;
	double divi = det3(a, b, c);
	ans.x = det3(d, b, c) / divi;
	ans.y = det3(a, d, c) / divi;
	ans.z = det3(a, b, d) / divi;
	return ans;
}