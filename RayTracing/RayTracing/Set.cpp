#include "Set.h"

#define MAXINF 1e+20
#define EPS 1e-6

Set::Set() {
	objSet = new vector<Object*>();
}


Set::~Set() {
	for (vector<Object*>::iterator i = objSet->begin(); i != objSet->end(); i++)
		delete (*i);
	delete objSet;
}

void Set::addObj(Object *obj) {
	objSet->push_back(obj);
}

bool Set::intercept(const Ray& r, Vector& p, Object *(&obj)) {
	double minDis = MAXINF; obj = nullptr;
	Vector nowCrossPoint;
	for (vector<Object*>::iterator i = objSet->begin(); i != objSet->end(); i++) {
		if ((*i)->intercept(r, nowCrossPoint)) {
			if ((getDistance2(r.origin, nowCrossPoint) < minDis) && (getDistance2(r.origin, nowCrossPoint) > EPS)) {
				minDis = getDistance2(r.origin, nowCrossPoint);
				obj = *i;
				p = nowCrossPoint;
			}
		}
	}
	if (obj) return true; else return false;
}