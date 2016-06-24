#ifndef CAUSTICMAPPER_H
#define CAUSTICMAPPER_H

#include <thread>
#include <queue>
#include "photonmapper.h"

class CausticMapper : public PhotonMapper
{
public:
	CausticMapper();
	~CausticMapper();

	static void run();

	static void setR(Config* _conf);

	static void threadProc(int lightNo, int aimNo, int tot, vector<Photon*> *threadSet, int ran_seed);
	static bool photonWork(Photon *p);
	static bool objWork(Photon *p, Vector interceptP, Object* selected);
	static bool refractOutWork(Photon *p, Object *from);

	static Color photonColor(const Vector &p);

private:
	static Photon **photonArr;
	static int totPhoton;
	static PhotonNode *root;

	static double R2;

	static void traverseTree(const Vector &p, const PhotonNode *now, priority_queue<SimplePhoton> *selected);
};

#endif