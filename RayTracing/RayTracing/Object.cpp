#define EPS 1e-6

#include "Object.h"
#include "Sphere.h"
#include "Plane.h"
#include "RayTracer.h"
#include "Tools.h"

Object::Object() {
	type = "Object";
}

Object::Object(Vector _position, Color _color, double _diffuseFactor, double _specularFactor, int _specularPower,
		double _reflectFactor, double _environmentFactor, double _refractFactor, double _refractN) {
	type = "Object";
	position = _position, bgColor = _color, diffuseFactor = _diffuseFactor, specularFactor = _specularFactor, specularPower = _specularPower,
		reflectFactor = _reflectFactor, environmentFactor = _environmentFactor, refractFactor = _refractFactor, refractN = _refractN;
}

Object::~Object() {
}

bool Object::intercept(const Ray& r, Vector &p) {
	return false;
}

bool Object::getNormal(const Vector& p, Vector &N) {
	return false;
}
