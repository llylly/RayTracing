#include "Set.h"

#define MAXINF 1e+20
#define EPS 1e-6
#define INF 1e+20
#define DELTA 1e-5

Set::Set() {
	objSet = new vector<Object*>();
	KDRoot = 0;
}


Set::~Set() {
	for (vector<Object*>::iterator i = objSet->begin(); i != objSet->end(); i++)
		delete (*i);
	delete objSet;
}

void Set::addObj(Object *obj) {
	objSet->push_back(obj);
}
/*
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
*/

bool Set::intercept(const Ray& r, Vector &p, Object *(&obj)) {
	obj = 0;
	KDNode *n = KDRoot; 
	set<Mesh*> *foundRec = new set<Mesh*>();
	foundRec->clear();
	Vector nowCrossPoint;
	double minDis = INF;
	Vector point;
	Ray treeR = r;
	if (!Enclosure::intercept(n->box, treeR, point)) {
		delete foundRec;
		return false;
	}
	if (!Enclosure::inBox(n->box, treeR.origin))
		treeR.origin = point + DELTA * treeR.direction;
	while (n) {
		while (n->direc != 3) {
			if (n->direc == 0) {
				if (treeR.origin.x <= n->left->box.Xmax)
					n = n->left;
				else
					n = n->right;
			} else if (n->direc == 1) {
				if (treeR.origin.y <= n->left->box.Ymax)
					n = n->left;
				else
					n = n->right;
			} else if (n->direc == 2) {
				if (treeR.origin.z <= n->left->box.Zmax)
					n = n->left;
				else
					n = n->right;
			}
		}
		//if (Enclosure::minDis2(r.origin, n->box) > minDis) break;
		for (vector<Mesh*>::iterator i = n->objs.begin(); i != n->objs.end(); i++) 
			if (foundRec->count(*i) == 0) {
				foundRec->insert(*i);
				if ((*i)->intercept(r, nowCrossPoint)) {
					if ((getDistance2(r.origin, nowCrossPoint) < minDis) && (getDistance2(r.origin, nowCrossPoint) > EPS)) {
						minDis = getDistance2(r.origin, nowCrossPoint);
						obj = *i;
						p = nowCrossPoint;
					}
				}
			}
		Enclosure::intercept(n->box, treeR, point);
		treeR.origin = point + DELTA * treeR.direction;
		while ((n) && (!Enclosure::inBox(n->box, treeR.origin))) {
			n = n->father;
		}
	}
	delete foundRec;
	if (obj) return true; else return false;
}

void Set::buildKDTree() {
	box.Xmax = -INF, box.Xmin = INF, box.Ymax = -INF, box.Ymin = INF, box.Zmax = -INF, box.Zmin = INF;
	for (vector<Object*>::iterator i = objSet->begin(); i != objSet->end(); i++) {
		((Mesh*)(*i))->calEnclosure();
		box += *(((Mesh*)(*i))->box);
	}
	KDRoot = new KDNode(box, 0, 0);
	for (vector<Object*>::iterator i = objSet->begin(); i != objSet->end(); i++)
		KDRoot->addMesh((Mesh*)(*i));
	KDRoot->gen();
}