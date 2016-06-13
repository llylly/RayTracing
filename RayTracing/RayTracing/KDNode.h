#ifndef KDNODE_H
#define KDNODE_H

#include "Enclosure.h"
#include "Object.h"
#include "Mesh.h"
#include "KDBuilder.h"
#include <set>

class KDNode
{
public:
	static int tot;

	Enclosure box;
	vector<Mesh*> objs;
	int direc, depth;
	KDNode *left, *right, *father;

	KDNode();
	KDNode(const Enclosure &_box, int _direc, int _depth);
	~KDNode();

	void addMesh(Mesh* _mesh);
	void gen();
};

#endif