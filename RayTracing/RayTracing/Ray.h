#ifndef RAY_H
#define RAY_H

#include "Vector.h"

class Ray {
public:
	Vector origin, direction;

	Ray();
	Ray(Vector, Vector);
	~Ray();
};

#endif