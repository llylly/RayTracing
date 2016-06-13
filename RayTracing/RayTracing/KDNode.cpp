#include "KDNode.h"

int KDNode::tot = 0;

KDNode::KDNode() {
	objs.clear();
	direc = depth = 0;
	left = right = father = 0;
	box = Enclosure();
}

KDNode::KDNode(const Enclosure &_box, int _direc, int _depth) {
	box = _box;
	direc = _direc;
	depth = _depth;
	objs.clear();
	left = right = father = 0;
}

KDNode::~KDNode() {
}

void KDNode::addMesh(Mesh *_mesh) {
	objs.push_back(_mesh);
}

void KDNode::gen() {
	KDBuilder *builder = new KDBuilder();
	bool hasSon = builder->work(this);
	delete builder;
	if (hasSon) {
		left->gen();
		right->gen();
	}
}