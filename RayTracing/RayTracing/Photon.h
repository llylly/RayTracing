#ifndef PHOTON_H
#define PHOTON_H

#include "Ray.h"
#include "Vector.h"
#include "Color.h"

class Photon: public Ray
{
public:
	Photon();
	Photon(const Vector& _pos, const Vector& _direc, const Color &_color);
	~Photon();

	Color color;
};

#endif
