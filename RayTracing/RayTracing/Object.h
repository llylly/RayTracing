#ifndef OBJECT_H
#define OBJECT_H

#include "Color.h"
#include "Ray.h"
#include "Vector.h"

struct RayTracer;

class Object {
public:
	string type;
	Vector position;
	Color bgColor;
	double diffuseFactor, specularFactor;
	int specularPower;
	double reflectFactor, environmentFactor;

	Object();
	Object(Vector _position, Color _color, double _diffuseFactor, double _specularFactor, int _specularPower,
		double _reflectFactor, double _environmentFactor);
	~Object();

	virtual bool intercept(const Ray&, Vector&);
	virtual Color work(RayTracer* rayTracer, const Ray& inRay, const Vector& interceptP, double co);

private:
	virtual bool getNormal(const Vector&, Vector&);
};

#endif