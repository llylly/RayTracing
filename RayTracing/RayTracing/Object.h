#ifndef OBJECT_H
#define OBJECT_H

#include "Color.h"
#include "Ray.h"
#include "Vector.h"
#include "BMP.h"

struct RayTracer;

class Object {
public:
	string type;
	Vector position;
	Color bgColor;
	bool textured;
	Vector textureOrigin, textureXVec, textureYVec;
	string texturePath;
	double diffuseFactor, specularFactor;
	int specularPower;
	double reflectFactor, diffuseReflectValue, environmentFactor;
	double refractFactor, refractN, beerConst;

	double textureXVecLen, textureYVecLen;
	BMP texture;

	Object();
	Object(Vector _position, Color _color, bool _textured, Vector _textureOrigin, Vector _textureXVec, Vector _textureYVec, string _texturePath, double _diffuseFactor, double _specularFactor, int _specularPower,
		double _reflectFactor, double _diffuseReflectValue, double _environmentFactor, double _refractFactor, double _refractN, double _beerConst);
	~Object();

	virtual bool intercept(const Ray&, Vector&);
	virtual bool getNormal(const Vector&, Vector&);
	virtual Color getColor(const Vector& p);

	void calTextureVec(const Vector&);
};

#endif