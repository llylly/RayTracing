#ifndef SPHERE_H
#define SPHERE_H

#include "Color.h"
#include "Vector.h"
#include "Object.h"
#include "Ray.h"

class Sphere : public Object {
public:
	double radius;

	Sphere();
	Sphere(Vector _position, Color _bgColor, bool _textured, Vector _textureOrigin, Vector _textureXVec, Vector _textureYVec, BMP *_texture, double _radius, 
		double _diffuseFactor, double _specularFactor, int _specularPower, 
		double _reflectFactor, double _diffuseReflectValue, double _environmentFactor, double _refractFactor, double _refractN, double _beerConst);
	~Sphere();

	bool intercept(const Ray&, Vector&) override;
	bool getNormal(const Vector &p, Vector &N) override;
	Color getColor(const Vector& p) override;
};

#endif