#include "Light.h"


Light::Light() {
	type = "Light";
}

Light::~Light() {
}

Color Light::haloColor(const Ray &r) {
	return Color(0.0f, 0.0f, 0.0f);
}