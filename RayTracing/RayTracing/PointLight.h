#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "Vector.h"
#include "Light.h"
#include "Color.h"

class PointLight : public Light {
public:
	Vector position;
	Color color;
	int haloFactor;

	PointLight();
	~PointLight();
	PointLight(Vector, Color, int);

	Color haloColor(const Ray&) override;
};

#endif