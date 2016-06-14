#ifndef ENCLOSURE_H
#define ENCLOSURE_H

#include "Vector.h"
#include "Ray.h"

class Enclosure {
public:
	Enclosure();
	Enclosure(double, double, double, double, double, double);
	~Enclosure();

	double Xmin, Xmax, Ymin, Ymax, Zmin, Zmax;

	bool inBox(const Vector&);
	Enclosure &operator+=(const Enclosure&);
	Enclosure &operator+=(const Vector&);

	double area();

	static bool intercept(const Enclosure &e, const Ray &r, Vector &p);
	static bool inBox(const Enclosure &e, const Vector &p);
	static double minDis2(const Vector &p, const Enclosure &e);
};

Enclosure operator+(const Enclosure&, const Enclosure&);

#endif
