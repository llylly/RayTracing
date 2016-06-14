#include "PhotonNode.h"

PhotonNode::PhotonNode() {
	parent = left = right = 0;
	photons = vector<Photon*>();
}

PhotonNode::~PhotonNode() {
}
