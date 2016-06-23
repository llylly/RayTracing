#ifndef RAY_TRACER_H
#define RAY_TRACER_H

#include <string>
#include <vector>
#include <thread>
#include "ConfigReader.h"
#include "Config.h"
#include "Image.h"
#include "Camera.h"
#include "Vector.h"
#include "ObjectFactory.h"
#include "Object.h"
#include "LightFactory.h"
#include "Light.h"
#include "PlaneLight.h"
#include "SetFactory.h"
#include "Set.h"
#include "Ray.h"
#include "Set.h"

using namespace std;

class RenderView;

struct RayTracer {
public:
	static string configIn, imageOut;
	static vector<Light*> lights;

	RayTracer();
	RayTracer(string configIn, string imageOut);
	~RayTracer();
	static void run();
	static void save();
	static Color work(const Ray&, double co = 1.0f);
	static bool collide(const Vector&, const Vector&);

	static int getRenderWidth();
	static int getRenderHeight();
	static bool getDisplayOn();

private:
	static Config *config;
	static Image *image;
	static Camera *camera;
	static vector<Object*> objects;
	static vector<Set*> sets;

	static void threadProc(int s, int ran_seed);

	static Color objWork(const Ray& r, Vector crossPoint, double co, Object* selected);
	static Color refractOutWork(const Ray& r, double co);

	static bool getCrossedObj(const Ray &r, Vector& crossPoint, Object *(&crossObj), Light *(&crossLight));

	static Config *getConfig();
	static Image *getImage();
	static vector<Light*> *getLights();
	static Camera *getCamera();
	static vector<Object*> *getObjects();
	static vector<Set*> *getSets();

	friend RenderView;
};
#endif