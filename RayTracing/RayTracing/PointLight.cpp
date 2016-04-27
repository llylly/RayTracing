#include "PointLight.h"

PointLight::PointLight() {
	type = "PointLight";
}

PointLight::~PointLight() {
}

PointLight::PointLight(Vector v, Color c, int h) {
	type = "PointLight";
	position = v, color = c, haloFactor = h;
}

Color PointLight::haloColor(const Ray &r) {
	double d = dot(normalize(position - r.origin), r.direction);
	if (d < 0.0f) return Color(0.0f, 0.0f, 0.0f);
	for (int i=0; i<haloFactor; i++) d*=d; // d_new = d^(2^haloFactor)
	return color * d;
}