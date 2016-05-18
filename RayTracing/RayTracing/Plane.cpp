#define EPS 1e-6

#include "Plane.h"
#include "RayTracer.h"

Plane::Plane() {
	type = "Plane";
}

Plane::Plane(Vector _N, Vector _position, Color _color, double _diffuseFactor, double _specularFactor, int _specularPower,
		double _reflectFactor, double _environmentFactor, double _refractFactor, double _refractN, double _beerConst) {
	type = "Plane";
	N = _N, 
	position = _position, bgColor = _color, diffuseFactor = _diffuseFactor, specularFactor = _specularFactor, specularPower = _specularPower,
				reflectFactor = _reflectFactor, environmentFactor = _environmentFactor,
				refractFactor = _refractFactor, refractN = _refractN, beerConst = _beerConst;
}

Plane::~Plane() {
}

bool Plane::intercept(const Ray &r, Vector &p) {
	if (abs(dot(N, r.direction))<EPS) return false;
	double T = dot(N, position - r.origin) / dot(N, r.direction);
	if (T < 0.0f) return false;
	p = r.origin + T * r.direction;
	return true;
}

bool Plane::getNormal(const Vector &p, Vector &N) {
	// guarantee p is on the place
	if (abs(dot(this->N, p - position))>EPS) return false;
	N = this->N;
	return true;
}