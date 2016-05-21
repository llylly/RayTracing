#ifndef VECTOR_H
#define VECTOR_H

#include <cmath>
#include <iostream>

using namespace std;

class Vector {
public:
	double x, y, z;

	Vector();
	Vector(double, double, double);
	Vector(const Vector&);
	Vector &operator=(const Vector&);
};

Vector operator*(double, const Vector&);

Vector operator*(const Vector&, double);

Vector &operator*=(Vector&, double);

Vector operator/(const Vector&, double);

Vector &operator/=(Vector&, double);

Vector operator+(const Vector&, const Vector&);

Vector &operator+=(Vector&, const Vector&);

Vector operator-(const Vector&, const Vector&);

Vector &operator-=(Vector&, const Vector&);

double dot(const Vector&, const Vector&);

Vector cross(const Vector&, const Vector&);

double getLength(const Vector&);

Vector normalize(const Vector &v, double toLen = 1.0);

ostream &operator<<(ostream &outS, const Vector&);

double getDistance(const Vector&, const Vector&);

double getDistance2(const Vector&, const Vector&);

Vector operator-(const Vector&);

Vector getArbitraryPlaneVec(const Vector &N, double length = 1.0f);

Vector getPerpendicularVec(const Vector &N, const Vector &planeVec);

double det3(const Vector &v1, const Vector &v2, const Vector &v3);

#endif