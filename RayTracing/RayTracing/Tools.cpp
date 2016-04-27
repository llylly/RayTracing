#include "Tools.h"

double powF(double x, int a) {
	double ans = 1.0f;
	while (a) {
		if (a&1) ans *= x;
		a>>=1, x*=x;
	}
	return ans;
}