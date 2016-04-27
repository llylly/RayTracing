#include "Vector.h"

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