#ifndef SET_H
#define SET_H

#include <vector>
#include "Color.h"
#include "Object.h"

class Set {
public:
	Color bgColor;
	double diffuseFactor, specularFactor;
	int specularPower;
	double reflectFactor, environmentFactor;
	double refractFactor, refractN, beerConst;

	Set();
	~Set();
	void addObj(Object *obj);

private:
	vector<Object*> *objSet;

	friend class SetFactory;
	friend struct RayTracer;
};

#endif

