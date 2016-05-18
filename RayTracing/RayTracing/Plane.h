#ifndef PLANE_H
#define PLANE_H

#include "Object.h"
#include "Vector.h"
#include "Color.h"

class Plane : public Object {
public:
	Vector N;

	Plane();
	Plane(Vector _N, Vector _position, Color _color, double _diffuseFactor, double _specularFactor, int _specularPower,
		double _reflectFactor, double _environmentFactor, double _refractFactor, double _refractN, double _beerConst);
	~Plane();

	bool intercept(const Ray&, Vector&) override;

private:
	bool getNormal(const Vector&, Vector&) override;

	friend Object;
};

#endif
