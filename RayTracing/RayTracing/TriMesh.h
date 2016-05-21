#ifndef TRIMESH_H
#define TRIMESH_H

#include "mesh.h"

class TriMesh : public Mesh {
public:
	TriMesh();
	TriMesh(Vector _normal, vector<Vector> *_points, int _tot, Color _color, bool _textured, Vector _textureOrigin, Vector _textureXVec, Vector _textureYVec, string _texturePath, double _diffuseFactor, double _specularFactor, int _specularPower,
		double _reflectFactor, double _diffuseReflectValue, double _environmentFactor, double _refractFactor, double _refractN, double _beerConst, bool _recalNormal = true);
	~TriMesh();

	bool intercept(const Ray &r, Vector &p) override;

private:
	//double XFac, YFac, ZFac;
};

#endif