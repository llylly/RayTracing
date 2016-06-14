#include "Photon.h"

Photon::Photon()
{
}

Photon::~Photon()
{
}

Photon::Photon(const Vector &_pos, const Vector &_direc, const Color &_color) {
	origin = _pos, direction = _direc, color = _color;
}
