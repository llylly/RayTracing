#define EPS 1e-4

#include <map>
#include <vector>
#include "Sphere.h"
#include "RayTracer.h"
#include "Light.h"
#include "PointLight.h"
#include "Tools.h"

Sphere::Sphere() {
	type = "Sphere";
}

Sphere::Sphere(Vector _position, Color _bgColor, double _radius, 
		double _diffuseFactor, double _specularFactor, int _specularPower, 
		double _reflectFactor, double _environmentFactor,
		double _refractFactor, double _refractN) {
	type = "Sphere";
	position = _position, bgColor = _bgColor, radius = _radius, 
	diffuseFactor = _diffuseFactor, specularFactor = _specularFactor, specularPower = _specularPower,
		reflectFactor = _reflectFactor, environmentFactor = _environmentFactor,
		refractFactor = _refractFactor, refractN = _refractN;
}

Sphere::~Sphere() {
}

bool Sphere::intercept(const Ray &r, Vector &p) {
	Vector a = position - r.origin;
	if (getLength(a) >= radius) {
		double foot = dot(a, r.direction);
		if (foot < 0.0f) return false;
		double dis2;
		if ((dis2 = getDistance2(position, r.origin) - foot * foot) > radius * radius)
			return false;
		p = r.origin + r.direction * (foot - sqrt(radius * radius - dis2));
		return true;
	} else {
		double foot = dot(a, r.direction);
		double dis2 = getLength(a);
		p = r.origin + r.direction * (foot + sqrt(radius * radius - dis2 * dis2 + foot * foot));
		return true;
	}
}

bool Sphere::getNormal(const Vector& p, Vector& N) {
	N = p - position;
	if ((getDistance2(p, position) >= radius * radius + EPS) || (getDistance2(p, position) <= radius * radius -EPS)) 
		return false;
	return true;
}

