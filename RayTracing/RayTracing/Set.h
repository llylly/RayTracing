#ifndef SET_H
#define SET_H

#include <vector>
#include "Color.h"
#include "Object.h"

class Set {
public:
	Color bgColor;
	bool textured;
	string texturePath;
	double diffuseFactor, specularFactor;
	int specularPower;
	double reflectFactor, diffuseReflectValue, environmentFactor;
	double refractFactor, refractN, beerConst;

	string filePath;

	double scale;
	Vector displace;
	double XRotateAngle, YRotateAngle, ZRotateAngle;

	Set();
	~Set();
	void addObj(Object *obj);

	bool intercept(const Ray&, Vector&, Object *(&obj));

private:
	vector<Object*> *objSet;

	friend class SetFactory;
};

#endif

