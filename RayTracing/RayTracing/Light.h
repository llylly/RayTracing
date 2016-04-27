#ifndef LIGHT_H
#define LIGHT_H

#include "Color.h"
#include "Ray.h"

class Light {
public:
	Light();
	~Light();

	virtual Color haloColor(const Ray&);
	string type;
};

#endif