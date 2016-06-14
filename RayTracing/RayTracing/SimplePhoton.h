#ifndef SIMPLEPHOTON_H
#define SIMPLEPHOTON_H

#include "Vector.h"
#include "Photon.h"
#include "Color.h"

class SimplePhoton
{
public:
	SimplePhoton();
	~SimplePhoton();
	SimplePhoton(const Photon &p, const Vector &_origin);

	Vector position, origin;
	double len;
	Color c;
};

bool operator<(const SimplePhoton &a, const SimplePhoton &b);

#endif 
