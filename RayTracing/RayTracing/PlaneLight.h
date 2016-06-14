#ifndef PLANELIGHT_H
#define PLANELIGHT_H

#include "light.h"
#include "Vector.h"

class PlaneLight : public Light {
public:
	PlaneLight();
	PlaneLight(Vector _origin, Vector _xVec, Vector _yVec, Vector _normal, int _xDiv, int _yDiv, Color _color);
	~PlaneLight();

	Vector origin, xVec, yVec;
	Vector N;
	int xDiv, yDiv;
	Color color;

	Vector getRandomPos(int, int);
	bool intercept(const Ray&, Vector&);
};

#endif