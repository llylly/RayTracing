#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "Vector.h"
#include "Light.h"

class PointLight : public Light {
public:
	Vector position;

	PointLight();
	~PointLight();
	PointLight(Vector);
};

#endif