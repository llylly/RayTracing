#ifndef PLANE_H
#define PLANE_H

#include "Object.h"
#include "Vector.h"
#include "Color.h"

class Plane : public Object {
public:
	Vector N;

	Plane();
	Plane(Vector _N, Vector _position, Color _color, bool _textured, Vector _textureOrigin, Vector _textureXVec, Vector _textureYVec, BMP *_texture, double _diffuseFactor, double _specularFactor, int _specularPower,
		double _reflectFactor, double _diffuseReflectValue, double _environmentFactor, double _refractFactor, double _refractN, double _beerConst);
	~Plane();

	bool intercept(const Ray&, Vector&) override;
	bool getNormal(const Vector&, Vector&) override;
	Color getColor(const Vector& p) override;

};

#endif
