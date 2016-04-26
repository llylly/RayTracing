#ifndef RAY_TRACER_H
#define RAY_TRACER_H

#include <string>
#include <vector>
#include "ConfigReader.h"
#include "Config.h"
#include "Image.h"
#include "Camera.h"
#include "Vector.h"
#include "ObjectFactory.h"
#include "Object.h"
#include "LightFactory.h"
#include "Light.h"

using namespace std;

struct RayTracer {
public:
	string configIn, imageOut;

	RayTracer();
	RayTracer(string configIn, string imageOut);
	~RayTracer();
	void run();
	void save();

	int getRenderWidth();
	int getRenderHeight();
	bool getDisplayOn();

private:
	Config *config;
	Image *image;
	Camera *camera;
	vector<Object*> objects;
	vector<Light*> lights;
};
#endif