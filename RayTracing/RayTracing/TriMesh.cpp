#define EPS 1e-6

#include "TriMesh.h"

TriMesh::TriMesh(): Mesh() {
	type = "TriMesh";
}

TriMesh::TriMesh(Vector _normal, vector<Vector> *_points, int _tot, Color _color, bool _textured, Vector _textureOrigin, Vector _textureXVec, Vector _textureYVec, string _texturePath, double _diffuseFactor, double _specularFactor, int _specularPower,
		double _reflectFactor, double _diffuseReflectValue, double _environmentFactor, double _refractFactor, double _refractN, double _beerConst, bool _recalNormal)
		: Mesh(_normal, _points, _tot, _color, _textured, _textureOrigin, _textureXVec, _textureYVec, _texturePath, _diffuseFactor, _specularFactor, _specularPower,
		_reflectFactor, _diffuseReflectValue, _environmentFactor, _refractFactor, _refractN, _beerConst, _recalNormal) {
	type = "TriMesh";
	//XFac = (points->at(0) - points->at(1)).y * (points->at(0) - points->at(2)).z - (points->at(0) - points->at(1)).z * (points->at(0) - points->at(2)).y;
	//YFac = (points->at(0) - points->at(1)).z * (points->at(0) - points->at(2)).x - (points->at(0) - points->at(1)).x * (points->at(0) - points->at(2)).z;
	//ZFac = (points->at(0) - points->at(1)).x * (points->at(0) - points->at(2)).y - (points->at(0) - points->at(1)).y * (points->at(0) - points->at(2)).x;
}
	

TriMesh::~TriMesh() {
}

bool TriMesh::intercept(const Ray &r, Vector &p) {
	double c = det3(r.direction, points->at(0) - points->at(1), points->at(0) - points->at(2));
	//double c = r.direction.x * XFac + r.direction.y * YFac + r.direction.z * ZFac;
	if (abs(c) < EPS) return false;
	double t = det3(points->at(0) - r.origin, points->at(0) - points->at(1), points->at(0) - points->at(2)) / c;
	//double t = ((points->at(0) - r.origin).x * XFac + (points->at(0) - r.origin).y * YFac + (points->at(0) - r.origin).z * ZFac) / c;
	double beta = det3(r.direction, points->at(0) - r.origin, points->at(0) - points->at(2)) / c;
	double gama = det3(r.direction, points->at(0) - points->at(1), points->at(0) - r.origin) / c;
	if ((t > 0.0f) && (beta > 0.0f) && (gama > 0.0f) && (beta + gama < 1.0f)) {
		p = r.origin + t * r.direction;
		return true;
	} else
		return false;
}