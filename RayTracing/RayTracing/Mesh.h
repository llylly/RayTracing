#ifndef MESH_H
#define MESH_H

#include <vector>
#include "object.h"
#include "Vector.h"
#include "Enclosure.h"
#include "Tools.h"

class Mesh : public Object {
public:
	Vector N, normal; // normal is real normal by calculating, while N is given normal with disturbance
	vector<Vector> *points;
	int pointTot;
	Enclosure *box;

	//double textureXVecLen2, textureYVecLen2;

	Mesh();
	Mesh(Vector _normal, vector<Vector> *_points, int _tot, Color _color, bool _textured, Vector _textureOrigin, Vector _textureXVec, Vector _textureYVec, BMP *_texture, double _diffuseFactor, double _specularFactor, int _specularPower,
		double _reflectFactor, double _diffuseReflectValue, double _environmentFactor, double _refractFactor, double _refractN, double _beerConst, bool _recalNormal = true);
	~Mesh();

	virtual bool intercept(const Ray &r, Vector &p) override;
	bool getNormal(const Vector &p, Vector &N) override;
	Color getColor(const Vector& p) override;
	void calEnclosure();
};

#endif MESH_H