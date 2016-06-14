#include "SimplePhoton.h"

SimplePhoton::SimplePhoton() {
}

SimplePhoton::~SimplePhoton() {
}

SimplePhoton::SimplePhoton(const Photon& p, const Vector &_origin) {
	position = p.origin, origin = _origin;
	c = p.color;
	len = getDistance2(position, origin);
}

bool operator<(const SimplePhoton &a, const SimplePhoton &b) {
	return a.len < b.len;
}