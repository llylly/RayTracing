#ifndef MESH_H
#define MESH_H

#include <vector>
#include "object.h"
#include "Vector.h"

class Mesh : public Object {
public:
	Vector N, normal; // normal is real normal by calculating, while N is given normal with disturbance
	vector<Vector> *points;
	int pointTot;

	Mesh();
	Mesh(Vector _normal, vector<Vector> *_points, int _tot, Color _color, double _diffuseFactor, double _specularFactor, int _specularPower,
		double _reflectFactor, double _environmentFactor, double _refractFactor, double _refractN, double _beerConst, bool _recalNormal = true);
	~Mesh();

	bool intercept(const Ray &r, Vector &p);
	
private:
	bool getNormal(const Vector &p, Vector &N);

	friend Object;
};

#endif MESH_H