#ifndef PLANE_H
#define PLANE_H

#include "Object.h"
#include "Vector.h"

class Plane : public Object {
public:
	Vector N, point;

	Plane();
	Plane(Vector _N, Vector _point);
	~Plane();
};

#endif
