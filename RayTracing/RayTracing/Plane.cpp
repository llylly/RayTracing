#define EPS 1e-6

#include "Plane.h"
#include "RayTracer.h"
#include "Tools.h"

Plane::Plane() {
	type = "Plane";
}

Plane::Plane(Vector _N, Vector _position, Color _color, bool _textured, Vector _textureOrigin, Vector _textureXVec, Vector _textureYVec, BMP *_texture, double _diffuseFactor, double _specularFactor, int _specularPower,
		double _reflectFactor, double _diffuseReflectValue, double _environmentFactor, double _refractFactor, double _refractN, double _beerConst) {
	type = "Plane";
	N = _N, 
	position = _position, bgColor = _color, textured = _textured, textureOrigin = _textureOrigin, textureXVec = _textureXVec, textureYVec = _textureYVec, texture = _texture,
	diffuseFactor = _diffuseFactor, specularFactor = _specularFactor, specularPower = _specularPower,
	reflectFactor = _reflectFactor, diffuseReflectValue = _diffuseReflectValue, environmentFactor = _environmentFactor,
				refractFactor = _refractFactor, refractN = _refractN, beerConst = _beerConst;
	if (textured) {
		calTextureVec(N);
		textureXVecLen = getLength(textureXVec),
			textureYVecLen = getLength(textureYVec);
		textureOrigin += dot(_position - textureOrigin, N) * N;
	}
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

Color Plane::getColor(const Vector &p) {
	if (textured) {
		Vector ans = triEquationSolver(textureXVec, textureYVec, N, p - textureOrigin);
		return texture->getColor(ans.x, ans.y);
	} else 
		return bgColor;
}