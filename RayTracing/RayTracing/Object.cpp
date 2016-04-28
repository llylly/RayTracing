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
		double _reflectFactor, double _environmentFactor) {
	type = "Object";
	position = _position, bgColor = _color, diffuseFactor = _diffuseFactor, specularFactor = _specularFactor, specularPower = _specularPower,
		reflectFactor = _reflectFactor, environmentFactor = _environmentFactor;
}

Object::~Object() {
}

bool Object::intercept(const Ray& r, Vector &p) {
	return false;
}

bool Object::getNormal(const Vector& p, Vector &N) {
	return false;
}

Color Object::work(RayTracer* rayTracer, const Ray& inRay, const Vector& interceptP, double co) {
	Color ans = Color(0.0f, 0.0f, 0.0f);
	Vector normal;

	this->getNormal(interceptP, normal);

	if (environmentFactor > EPS) {
		ans += environmentFactor * bgColor;
	}
	for (vector<Light*>::const_iterator i = rayTracer->lights.begin(); i != rayTracer->lights.end(); i++) 
		if ((*i)->type == "PointLight") {
			if (rayTracer->collide(((PointLight*)(*i))->position, interceptP)) continue;
			if (diffuseFactor > EPS) {
				double d = dot(normalize(((PointLight*)(*i))->position - interceptP), normal);
				if (d > 0.0f)
					ans += diffuseFactor * d * bgColor * ((PointLight*)(*i))->color;
			}
			if (specularFactor > EPS) {
				Vector reflectDirec = 2.0f * dot(-inRay.direction, normal) * normal + inRay.direction;
				double f = dot(normalize(((PointLight*)(*i))->position - interceptP), reflectDirec);
				if (f > 0.0f) {
					f = powF(f, specularPower);
					ans += specularFactor * f * ((PointLight*)(*i))->color;
				}
			}
		}
	if (reflectFactor > EPS) {
		Vector reflectDirec = 2.0f * dot(-inRay.direction, normal) * normal + inRay.direction;
		Ray reflectRay(interceptP, normalize(reflectDirec));
		ans += reflectFactor * rayTracer->work(reflectRay, co * reflectFactor);
	}
	return ans;
}
