#include "Vector.h"
#define EPS 1e-6

Vector::Vector() {
}

Vector::Vector(double _x, double _y, double _z) {
	x = _x, y = _y, z = _z;
}

Vector::Vector(const Vector &a) {
	x = a.x, y = a.y, z = a.z;
}

Vector &Vector::operator=(const Vector &a) {
	x= a.x, y = a.y, z = a.z;
	return *this;
}

Vector operator*(double _k, const Vector &a) {
	Vector n(a.x * _k, a.y * _k, a.z * _k);
	return n;
}

Vector operator*(const Vector &a, double _k) {
	Vector n(a.x * _k, a.y * _k, a.z * _k);
	return n;
}

Vector &operator*=(Vector &a, double _k) {
	a.x *= _k, a.y *= _k, a.z *= _k;
	return a;
}

Vector operator/(const Vector &a, double _k) {
	Vector n(a.x / _k, a.y / _k, a.z / _k);
	return n;
}

Vector &operator/=(Vector &a, double _k) {
	a.x /= _k, a.y /= _k, a.z /= _k;
	return a;
}

Vector operator+(const Vector &a, const Vector &b) {
	Vector n(a.x + b.x, a.y + b.y, a.z + b.z);
	return n;
}

Vector &operator+=(Vector &a, const Vector &b) {
	a.x += b.x, a.y += b.y, a.z += b.z;
	return a;
}

Vector operator-(const Vector &a, const Vector &b) {
	Vector n(a.x - b.x, a.y - b.y, a.z - b.z);
	return n;
}

Vector &operator-=(Vector &a, const Vector &b) {
	a.x -= b.x, a.y -= b.y, a.z -= b.z;
	return a;
}

double dot(const Vector &a, const Vector &b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vector cross(const Vector &a, const Vector &b) {
	Vector n(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
	return n;
}

double getLength(const Vector &v) {
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

Vector normalize(const Vector &v, double toLen) {
	double k = toLen / getLength(v);
	Vector n = v * k;
	return n;
}

ostream &operator<<(ostream &outS, const Vector &a) {
	outS<<"("<<a.x<<", "<<a.y<<", "<<a.z<<")";
	return outS;
}

double getDistance(const Vector &a, const Vector &b) {
	return getLength(a - b);
}

double getDistance2(const Vector &a, const Vector &b) {
	return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z);
}

Vector operator-(const Vector &a) {
	return Vector(-a.x, -a.y, -a.z);
}

Vector getArbitraryPlaneVec(const Vector &N, double length) {
	Vector R;
	if (abs(N.z) > EPS) {
		R.x = (double)rand() / (double)RAND_MAX;
		R.y = (double)rand() / (double)RAND_MAX;
		R.z = -(N.x * R.x + N.y * R.y) / N.z;
	} else if (abs(N.y) > EPS) {
		R.x = (double)rand() / (double)RAND_MAX;
		R.z = (double)rand() / (double)RAND_MAX;
		R.y = -(N.x * R.x + N.z * R.z) / N.y;
	} else {
		R.y = (double)rand() / (double)RAND_MAX;
		R.z = (double)rand() / (double)RAND_MAX;
		R.x = -(N.y * R.y + N.z * R.z) / N.x;
	}
	return normalize(R, length);
}

Vector getPerpendicularVec(const Vector &N, const Vector &planeVec) {
	Vector ans = cross(N, planeVec);
	return normalize(ans, getLength(planeVec));
}

double det3(const Vector &v1, const Vector &v2, const Vector &v3) {
	return v1.x * v2.y * v3.z + v1.y * v2.z * v3.x + v1.z * v2.x * v3.y 
		- v1.x * v2.z * v3.y - v1.y * v2.x * v3.z - v1.z * v2.y * v3.x;
}