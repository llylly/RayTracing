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
	Sphere(Vector _position, Color _bgColor, double _radius, 
		double _diffuseFactor, double _specularFactor, int _specularPower, 
		double _reflectFactor, double _environmentFactor, double _refractFactor, double _refractN);
	~Sphere();

	bool intercept(const Ray&, Vector&) override;

private:
	bool getNormal(const Vector&, Vector&) override;

	friend Object;
};

#endif