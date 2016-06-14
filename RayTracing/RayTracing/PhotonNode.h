#ifndef PHOTONNODE_H
#define PHOTONNODE_H

#include <vector>
#include "Photon.h"
#include "Enclosure.h"

class PhotonNode {
public:
	PhotonNode();
	~PhotonNode();

	vector<Photon*> photons;
	Enclosure e;
	int direc;
	double border;
	PhotonNode *parent, *left, *right;
};

#endif
