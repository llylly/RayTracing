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

Sphere::Sphere(Vector _position, Color _bgColor, bool _textured, Vector _textureOrigin, Vector _textureXVec, Vector _textureYVec, BMP *_texture, double _radius, 
		double _diffuseFactor, double _specularFactor, int _specularPower, 
		double _reflectFactor, double _diffuseReflectValue, double _environmentFactor,
		double _refractFactor, double _refractN, double _beerConst,
		int _causticTot) {
	type = "Sphere";
	position = _position, bgColor = _bgColor, textured = _textured, textureOrigin = _textureOrigin, textureXVec = _textureXVec, textureYVec = _textureYVec, texture = _texture,
		radius = _radius, 
	diffuseFactor = _diffuseFactor, specularFactor = _specularFactor, specularPower = _specularPower,
		reflectFactor = _reflectFactor, diffuseReflectValue = _diffuseReflectValue, environmentFactor = _environmentFactor,
		refractFactor = _refractFactor, refractN = _refractN, beerConst = _beerConst;
	if (textured) {
		calTextureVec(Vector(0, 0, 1));
		textureXVecLen = getLength(textureXVec),
			textureYVecLen = getLength(textureYVec);
		textureOrigin.z = 0.0f;	
	}
	causticTot = _causticTot;
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

Color Sphere::getColor(const Vector &p) {
	if (textured) {
		Vector P = normalize(p - position);
		double X = dot(P, Vector(0.0f, 1.0f, 0.0f));
		double Y = dot(P, Vector(1.0f, 0.0f, 0.0f)) * sqrt(1.0f - X * X);
		return texture->getColor( X / 2.0f + 0.5f, Y / 2.0f + 0.5);
	} else 
		return bgColor;
}
