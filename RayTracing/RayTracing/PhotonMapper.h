#ifndef PHOTONMAPPER_H
#define PHOTONMAPPER_H

#include <thread>
#include <queue>
#include "Config.h"
#include "Image.h"
#include "Photon.h"
#include "PointLight.h"
#include "PlaneLight.h"
#include "PhotonNode.h"
#include "SimplePhoton.h"

class Object;
class Set;
class Camera;
class Light;

class PhotonMapper {
public:
	PhotonMapper();
	~PhotonMapper();

	static void setConfig(Config* _conf);
	static void setImage(Image* _image);
	static void setLights(vector<Light*> *_lights);
	static void setCamera(Camera *_camera);
	static void setObjects(vector<Object*> *_objects);
	static void setSets(vector<Set*> *_sets);
	
	static void run();

	static void threadProc(int s, int ran_seed, vector<Photon*> *threadSet);
	static bool photonWork(Photon* p);
	static bool objWork(Photon *p, Vector interceptP, Object* selected);
	static bool getCrossedObj(Photon *p, Vector& crossPoint, Object *(&crossObj));
	static bool refractOutWork(Photon *p);

	static void buildPhotonTree(PhotonNode *p, int l, int r);

	static Color photonColor(const Vector &p);

private:
	static Config* config;
	static Image* image;
	static vector<Light*> lights;
	static Camera* camera;
	static vector<Object*> objects;
	static vector<Set*> sets;

	static Photon **photonArr;
	static int totPhoton;
	static PhotonNode *root;

	static double R2;

	static void q_sort(int L, int R, int direc);
	static void traverseTree(const Vector &p, const PhotonNode *now, priority_queue<SimplePhoton> *selected);
};

#endif