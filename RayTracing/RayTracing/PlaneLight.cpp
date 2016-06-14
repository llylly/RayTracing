#define EPS 1e-6

#include "PlaneLight.h"

PlaneLight::PlaneLight() {
	xDiv = yDiv = 1;
	type = "PlaneLight";
}


PlaneLight::~PlaneLight() {
}

PlaneLight::PlaneLight(Vector _origin, Vector _xVec, Vector _yVec, Vector _normal, int _xDiv, int _yDiv, Color _color) {
	origin = _origin, xVec = _xVec, yVec = _yVec,
		N = _normal, xDiv = _xDiv, yDiv = _yDiv, color = _color;
	type = "PlaneLight";
}

Vector PlaneLight::getRandomPos(int x, int y) {
	Vector deltaX = xVec / (double)xDiv, deltaY = yVec / (double)yDiv;
	return origin + deltaX * (x-1) + deltaY * (y-1) 
		+ (double)rand() / (double)RAND_MAX * deltaX
		+ (double)rand() / (double)RAND_MAX * deltaY;
}

bool PlaneLight::intercept(const Ray &r, Vector &interP) {
	if (abs(dot(r.direction, N)) < EPS) return false;
	double a = dot(origin - r.origin, N) / dot(r.direction, N);
	if (a < 0) return false;
	interP = r.origin + a * r.direction;
	Vector R = interP - origin;
	double kx, ky;
	if (abs(xVec.x * yVec.y - xVec.y * yVec.x) > EPS) {
		kx = (R.x * yVec.y - R.y * yVec.x) / (xVec.x * yVec.y - xVec.y * yVec.x);
		ky = (xVec.x * R.y - xVec.y * R.x) / (xVec.x * yVec.y - xVec.y * yVec.x);
	} else
	if (abs(xVec.x * yVec.z - xVec.z * yVec.x) > EPS) {
		kx = (R.x * yVec.z - R.z * yVec.x) / (xVec.x * yVec.z - xVec.z * yVec.x);
		ky = (xVec.x * R.z - xVec.z * R.x) / (xVec.x * yVec.z - xVec.z * yVec.x);
	} else {
		kx = (R.y * yVec.z - R.z * yVec.y) / (xVec.y * yVec.z - xVec.z * yVec.y);
		ky = (xVec.y * R.z - xVec.z * R.y) / (xVec.y * yVec.z - xVec.z * yVec.y);
	}
	if ((kx >= 0.0f) && (kx <= 1.0f) && (ky >= 0.0f) && (ky <= 1.0f))
		return true;
	return false;
}