#define DEBUG

#define MAXINF 1e+20
#define EPS 1e-6
#include <string>
#include <iostream>
#include "RayTracer.h"
#include "RenderView.h"

using namespace std;

RayTracer::RayTracer() {
	config = new Config();
	image = new Image();
	objects.clear();
}

RayTracer::RayTracer(string configIn, string imageOut) {
	//setting files
	this->configIn = configIn;
	this->imageOut = imageOut;

	//setting configuration reader
	config = new Config();
	ConfigReader *configReader = new ConfigReader(configIn, config);
	configReader->readConfig();
	delete configReader;

	//setting image
	image = new Image(imageOut, config);
	image->spaceInit();

	//setting camera
	camera = new Camera(config->getCameraConf(), getRenderWidth(), getRenderHeight());
#ifdef DEBUG
	cout<<"------CAMERA------"<<endl;
	cout<<"Eye: "<<camera->eye<<endl;
	cout<<"Center: "<<camera->center<<endl;
	cout<<"XVector: "<<camera->xDirec<<endl;
	cout<<"YVector: "<<camera->yDirec<<endl;
	cout<<"------------------"<<endl<<endl;
#endif

	//setting objects
	ObjectFactory *objectFactory = new ObjectFactory();
	for (vector<map<string, string>>::const_iterator i = config->getObjectConf().begin(); i != config->getObjectConf().end(); i++)
		objects.push_back(objectFactory->newObj(*i));

	//setting lights
	LightFactory *lightFactory = new LightFactory();
	for (vector<map<string, string>>::const_iterator i = config->getLightConf().begin(); i != config->getLightConf().end(); i++)
		lights.push_back(lightFactory->newLight(*i));
}

RayTracer::~RayTracer() {
	delete config;
	delete image;
	delete camera;
}

void RayTracer::save() {
	image->antiAliasing();
	image->save();
}

int RayTracer::getRenderWidth() {
	return config->resolutionWidth * config->antiAliasingFactor;
}

int RayTracer::getRenderHeight() {
	return config->resolutionHeight * config->antiAliasingFactor;
}

bool RayTracer::getDisplayOn() {
	return config->displayOn;
}

void RayTracer::run() {
	double kX, kY;
	Vector half, origin = camera->center, direction;
	Ray nowRay;

	for (int i=0; i<getRenderHeight(); ++i) {
		cerr<<i<<endl;
		kY = 1.0f - 2.0f * (double)i / (double)getRenderHeight();
		half = origin - camera->eye - kY * camera->yDirec;
		for (int j=0; j<getRenderWidth(); ++j) {
			kX = 1.0f - 2.0f * (double)j / (double)getRenderWidth();
			direction = half - kX * camera->xDirec;
			nowRay = Ray(origin, normalize(direction));
			image->set(work(nowRay), j, i);
			if (config->displayOn) RenderView::imgDisplay(image, j, i);
		}
	}
}

Color RayTracer::work(const Ray& r, double co) {
	if (co < config->limitCoefficient) return Color(0.0f, 0.0f, 0.0f);

	Object *selected = nullptr;
	double minDis = MAXINF;
	Vector nowCrossPoint, totCrossPoint;
	for (vector<Object*>::iterator i = objects.begin(); i != objects.end(); i++) {
		if ((*i)->intercept(r, nowCrossPoint)) {
			if ((getDistance2(r.origin, nowCrossPoint) < minDis) && (getDistance2(r.origin, nowCrossPoint) > EPS)) {
				minDis = getDistance2(r.origin, nowCrossPoint);
				selected = *i;
				totCrossPoint = nowCrossPoint;
			}
		}
	}

	Color ans = Color(0.0f, 0.0f, 0.0f);

	if (selected != nullptr) {
		// have intercept point
		ans = selected->work(this, r, totCrossPoint, co);
	} else {
		// calculate only light source
		for (vector<Light*>::iterator i = lights.begin(); i != lights.end(); i++) 
			ans += (*i)->haloColor(r);
	}

	return ans;
}

bool RayTracer::collide(const Vector &S, const Vector &T) {
	Ray r(S, normalize(T-S));
	double minDis = MAXINF;
	Vector nowCrossPoint;
	for (vector<Object*>::iterator i = objects.begin(); i != objects.end(); i++) {
		if ((*i)->intercept(r, nowCrossPoint)) {
			if ((getDistance2(S, nowCrossPoint) < minDis) && (getDistance2(S, nowCrossPoint) > EPS))
				minDis= getDistance2(r.origin, nowCrossPoint);
		}
	}
	if ((minDis > EPS) && (getDistance2(S, T) - minDis > EPS)) 
		return true;
	else
		return false;
}
