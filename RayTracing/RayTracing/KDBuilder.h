#ifndef KDBUILDER_H
#define KDBUILDER_H

#include "Enclosure.h"
#include "Mesh.h"

class KDNode;

class KDBuilder
{
public:
	static const double C_traversal, C_intersect;

	KDBuilder();
	~KDBuilder();

	bool work(KDNode *parent);
};


#endif
