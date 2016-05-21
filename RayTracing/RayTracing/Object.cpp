#define EPS 1e-6

#include "Object.h"
#include "Sphere.h"
#include "Plane.h"
#include "RayTracer.h"
#include "Tools.h"

Object::Object() {
	type = "Object";
}

Object::Object(Vector _position, Color _color, bool _textured, Vector _textureOrigin, Vector _textureXVec, Vector _textureYVec, string _texturePath, double _diffuseFactor, double _specularFactor, int _specularPower,
		double _reflectFactor, double _diffuseReflectValue, double _environmentFactor, double _refractFactor, double _refractN, double _beerConst) {
	type = "Object";
	position = _position, bgColor = _color, textured = _textured, textureOrigin = _textureOrigin, textureXVec = _textureXVec, textureYVec = _textureYVec, texturePath = _texturePath,
		diffuseFactor = _diffuseFactor, specularFactor = _specularFactor, specularPower = _specularPower,
		reflectFactor = _reflectFactor, diffuseReflectValue = _diffuseReflectValue, environmentFactor = _environmentFactor, refractFactor = _refractFactor, refractN = _refractN, beerConst = _beerConst;
	if (textured) {
		textureXVecLen = getLength(_textureXVec),
			textureYVecLen = getLength(_textureYVec);
		texture.open(_texturePath);
		calTextureVec(Vector(0.0f, 0.0f, 0.0f));
	}
}

Object::~Object() {
}

bool Object::intercept(const Ray& r, Vector &p) {
	return false;
}

bool Object::getNormal(const Vector& p, Vector &N) {
	return false;
}

Color Object::getColor(const Vector& p) {
	return bgColor;
}

void Object::calTextureVec(const Vector &N) {
	if (getLength(textureXVec) < EPS)
		textureXVec = getArbitraryPlaneVec(N);
	if ((getLength(textureYVec) < EPS) || 
		(abs(dot(textureYVec, textureXVec)) > EPS) ||
		(abs(dot(textureYVec, N)) > EPS))
		textureYVec = getPerpendicularVec(N, textureXVec);
}