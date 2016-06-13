#ifndef SET_H
#define SET_H

#include <vector>
#include "Color.h"
#include "Object.h"
#include "Mesh.h"
#include "KDNode.h"
#include "Enclosure.h"

class Set {
public:

	Color bgColor;
	bool textured;
	BMP *texture;
	double diffuseFactor, specularFactor;
	int specularPower;
	double reflectFactor, diffuseReflectValue, environmentFactor;
	double refractFactor, refractN, beerConst;

	string filePath;

	double scale;
	Vector displace;
	double XRotateAngle, YRotateAngle, ZRotateAngle;

	KDNode *KDRoot;
	Enclosure box;

	Set();
	~Set();
	void addObj(Object *obj);

	bool intercept(const Ray&, Vector&, Object *(&obj));

	void buildKDTree();

private:
	vector<Object*> *objSet;

	friend class SetFactory;
};

#endif

