#include "Ray.h"

Ray::Ray() {
}

Ray::Ray(Vector _origin, Vector _direction) {
	origin = _origin, direction = _direction;
}

Ray::~Ray() {
}
