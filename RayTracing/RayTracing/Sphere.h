#ifndef SPHERE_H
#define SPHERE_H

#include "Color.h"
#include "Vector.h"
#include "Object.h"

class Sphere : public Object {
public:
	Vector position;
	Color bgColor;
	double radius;
	double absorbFactor, reflectFactor;

	Sphere();
	Sphere(Vector _position, Color _bgColor, double _radius, double _absorbFactor, double _reflectFactor);
	~Sphere();
};

#endif