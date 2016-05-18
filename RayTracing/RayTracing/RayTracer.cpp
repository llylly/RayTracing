#define DEBUG

#define MAXINF 1e+20
#define EPS 1e-6
#include <string>
#include <iostream>
#include "RayTracer.h"
#include "RenderView.h"
#include "Tools.h"

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

	//setting sets
	SetFactory *setFactory = new SetFactory();
	for (vector<pair<map<string, string>, vector<map<string, string>>>>::const_iterator i = config->getSetConf().begin(); i != config->getSetConf().end(); i++) 
		sets.push_back(setFactory->newSet(*i));

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
	Vector half, origin = camera->center, direction;
	Ray nowRay;

#pragma omp parallel for
	for (int i=0; i<getRenderHeight(); ++i) {
		cerr<<i<<endl;
		double kY = 1.0f - 2.0f * (double)i / (double)getRenderHeight();
		Vector half = origin - camera->eye - kY * camera->yDirec;
		for (int j=0; j<getRenderWidth(); ++j) {
			double kX = 1.0f - 2.0f * (double)j / (double)getRenderWidth();
			Vector direction = half - kX * camera->xDirec;
			Ray nowRay = Ray(origin, normalize(direction));
			image->set(work(nowRay), j, i);
			if (config->displayOn) RenderView::imgDisplay(image, j, i);
		}
	}
}

Color RayTracer::objWork(const Ray& r, Vector interceptP, double co, Object* selected) {
	Color ans = Color(0.0f, 0.0f, 0.0f);
	Vector normal;

	selected->getNormal(interceptP, normal);

	if (selected->environmentFactor > EPS) {
		ans += selected->environmentFactor * selected->bgColor;
	}
	for (vector<Light*>::const_iterator i = lights.begin(); i != lights.end(); i++) 
		if ((*i)->type == "PointLight") {
			if (collide(((PointLight*)(*i))->position, interceptP)) continue;
			if (selected->diffuseFactor > EPS) {
				double d = dot(normalize(((PointLight*)(*i))->position - interceptP), normal);
				if (d > 0.0f)
					ans += selected->diffuseFactor * d * selected->bgColor * ((PointLight*)(*i))->color;
			}
			if (selected->specularFactor > EPS) {
				Vector reflectDirec = 2.0f * dot(-r.direction, normal) * normal + r.direction;
				double f = dot(normalize(((PointLight*)(*i))->position - interceptP), reflectDirec);
				if (f > 0.0f) {
					f = powF(f, selected->specularPower);
					ans += selected->specularFactor * f * ((PointLight*)(*i))->color;
				}
			}
		}
	if (selected->reflectFactor > EPS) {
		Vector reflectDirec = 2.0f * dot(-r.direction, normal) * normal + r.direction;
		Ray reflectRay(interceptP, normalize(reflectDirec));
		ans += selected->reflectFactor * work(reflectRay, co * selected->reflectFactor);
	}
	if (selected->refractFactor > EPS) {
		double nInv = 1.0f / selected->refractN;
		double cosI = -dot(normal, r.direction);
		if (cosI < -EPS) 
			cosI = -cosI, nInv = selected->refractN;
		double cosT2 = 1.0f - nInv * nInv * (1.0f - cosI * cosI);
		if (cosT2 > 0.0f) {
			Vector T = (nInv * r.direction) + (nInv * cosI - sqrt(cosT2)) * normal;
			Ray refractRay(interceptP + T * EPS, T);
			ans += selected->refractFactor * refractOutWork(refractRay, co * selected->refractFactor);
		}
	}
	return ans;
}

Color RayTracer::refractOutWork(const Ray &r, double co) {
	Vector totCrossPoint;
	Object *selected;
	int times = 0;
	Ray nowR = r;
	double dist = 0.0f;
	while (times <= 100) { //parameter's misery
		if (getCrossedObj(nowR, totCrossPoint, selected)) {
			Vector normal;
			selected->getNormal(totCrossPoint, normal);
			dist += getDistance(nowR.origin, totCrossPoint);
			double nInv = 1.0f / selected->refractN;
			double cosI = -dot(normal, nowR.direction);
			if (cosI > 0) {
				double cosT2 = 1.0f - nInv * nInv * (1.0f - cosI * cosI);
				if (cosT2 > 0.0f) {
					Vector T = (nInv * nowR.direction) + (nInv * cosI - sqrt(cosT2)) * normal;
					Ray refractRay(totCrossPoint + T * EPS, T);
					return exp(-dist * selected->beerConst) * work(refractRay, co);
				}
			} else {
				double cosT2 = 1.0f - selected->refractN * selected->refractN * (1.0f - cosI * cosI);
				if (cosT2 > 0.0f) {
					Vector T = (selected->refractN * nowR.direction) + (sqrt(cosT2) + selected->refractN * cosI) * normal;
					Ray refractRay(totCrossPoint + T * EPS, T);
					return exp(-dist * selected->beerConst) * work(refractRay, co);
				}
			}
			++times;
			Vector reflectDirec = 2.0f * dot(-nowR.direction, normal) * normal + nowR.direction;
			nowR = Ray(totCrossPoint + reflectDirec * EPS, reflectDirec);
		} else 
			return Color(0.0f, 0.0f, 0.0f);
	}
	return Color(0.0f, 0.0f, 0.0f);
}

Color RayTracer::work(const Ray& r, double co) {
	if (co < config->limitCoefficient) return Color(0.0f, 0.0f, 0.0f);

	Vector totCrossPoint;
	Object *selected;

	Color ans = Color(0.0f, 0.0f, 0.0f);

	if (getCrossedObj(r, totCrossPoint, selected)) {
		// have intercept point
		ans = objWork(r, totCrossPoint, co, selected);
	} else {
		// calculate only light source
		for (vector<Light*>::iterator i = lights.begin(); i != lights.end(); i++) 
			ans += (*i)->haloColor(r);
	}

	return ans;
}

bool RayTracer::getCrossedObj(const Ray &r, Vector& crossPoint, Object *(&crossObj)) {
	crossObj = nullptr;
	double minDis = MAXINF;
	Vector nowCrossPoint;
	for (vector<Object*>::iterator i = objects.begin(); i != objects.end(); i++) {
		if ((*i)->intercept(r, nowCrossPoint)) {
			if ((getDistance2(r.origin, nowCrossPoint) < minDis) && (getDistance2(r.origin, nowCrossPoint) > EPS)) {
				minDis = getDistance2(r.origin, nowCrossPoint);
				crossObj = *i;
				crossPoint = nowCrossPoint;
			}
		}
	}
	for (vector<Set*>::iterator s = sets.begin(); s != sets.end(); s++)
		for (vector<Object*>::iterator i = (*s)->objSet->begin(); i != (*s)->objSet->end(); i++) {
			if ((*i)->intercept(r, nowCrossPoint)) {
				if ((getDistance2(r.origin, nowCrossPoint) < minDis) && (getDistance2(r.origin, nowCrossPoint) > EPS)) {
					minDis = getDistance2(r.origin, nowCrossPoint);
					crossObj = *i;
					crossPoint = nowCrossPoint;
				}
			}
		}
	if (crossObj == nullptr) return false; else return true;
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
	for (vector<Set*>::iterator s = sets.begin(); s != sets.end(); s++)
		for (vector<Object*>::iterator i = (*s)->objSet->begin(); i != (*s)->objSet->end(); i++) {
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
