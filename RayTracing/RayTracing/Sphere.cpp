#include "Sphere.h"

Sphere::Sphere() {
}

Sphere::Sphere(Vector _position, Color _bgColor, double _radius, double _absorbFactor, double _reflectFactor) {
	position = _position, bgColor = _bgColor, radius = _radius, absorbFactor = _absorbFactor, reflectFactor = _reflectFactor;
}

Sphere::~Sphere() {
}
