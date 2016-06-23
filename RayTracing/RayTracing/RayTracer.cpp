#define DEBUG

#define MAXINF 1e+20
#define EPS 1e-6
#define PI 3.1415926
#include <string>
#include <iostream>
#include "RayTracer.h"
#include "RenderView.h"
#include "Tools.h"

using namespace std;

string RayTracer::configIn, RayTracer::imageOut;
vector<Light*> RayTracer::lights;

Config *RayTracer::config = 0;
Image *RayTracer::image = 0;
Camera *RayTracer::camera = 0;
vector<Object*> RayTracer::objects;
vector<Set*> RayTracer::sets;

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

	//KDTreeBuild
	for (vector<Set*>::iterator i = sets.begin(); i != sets.end(); i++) {
		cerr << "Build Kd-tree for a set" <<endl;
		(*i)->buildKDTree();
		cout << (*i)->box.Xmin << " " << (*i)->box.Xmax << " " << (*i)->box.Ymin << " " << (*i)->box.Ymax << 
			" " << (*i)->box.Zmin << " " << (*i)->box.Zmax << endl;
	}
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
	Color centerC = PhotonMapper::photonColor(Vector(0.0f, -1.0f, -13.0f));
	Color tmpb = PhotonMapper::photonColor(Vector(0.0f, -1.0f, -7.0f));
	cerr << "centerC: " << centerC.R << " " << centerC.G << " " << centerC.B << endl;
	cerr << "tmpb: " << tmpb.R << " " << tmpb.G << " " << tmpb.B << endl;
	Vector half, origin = camera->center, direction;
	Ray nowRay;

	thread *threadSet = new thread[config->totThread];
	for (int i=0; i<config->totThread; i++) {
		threadSet[i] = thread(threadProc, i, rand());
	}
	for (int i=0; i<config->totThread; i++) {
		threadSet[i].join(); 
		cerr << "Thread " << i << " joined" << endl;
	}
	delete[] threadSet;
}

void RayTracer::threadProc(int s, int ran_seed) {
	srand(ran_seed);
	Vector origin = camera->center, direction;
	Ray nowRay;

	Color nowPixel;
	Vector focus, startP;
	double t1, t2;

	for (int i = s; i < getRenderHeight(); i += config->totThread) {
			cerr<<i<<endl;
			double kY = 1.0f - 2.0f * (double)i / (double)getRenderHeight();
			Vector half = origin - camera->eye - kY * camera->yDirec;
			for (int j=0; j<getRenderWidth(); ++j) {
				//cerr << i << " " << j << endl;
				double kX = 1.0f - 2.0f * (double)j / (double)getRenderWidth();
				Vector direction = half - kX * camera->xDirec;
				focus = camera->eye + camera->focalRate * direction;
				nowPixel = Color(0.0f, 0.0f, 0.0f);
				if (camera->focalR > EPS) {
					for (int k=0; k < camera->samplingN; ++k) {
						t1 = (double)rand() / (double)RAND_MAX;
						t2 = (double)rand() / (double)RAND_MAX * 2.0f * PI;
						startP = origin + t1 * cos(t2) * camera->deltaX + t1 * cos(t2) * camera->deltaY;
						nowRay = Ray(startP , normalize(focus - startP));
						nowPixel += work(nowRay);
					}
					nowPixel /= camera->samplingN;
				} else {
					nowPixel = work(Ray(origin, normalize(direction)));
				}
				image->set(nowPixel, j, i);
				//Ray nowRay = Ray(origin, normalize(direction));
				//image->set(work(nowRay), j, i);
				//if (config->displayOn) RenderView::imgDisplay(image, j, i);
			}
	}
}

Color RayTracer::objWork(const Ray& r, Vector interceptP, double co, Object* selected) {
	Color ans(0.0f, 0.0f, 0.0f);
	Vector normal;

	selected->getNormal(interceptP, normal);

	if (selected->environmentFactor > EPS) {
		ans += selected->environmentFactor * selected->getColor(interceptP);
	}
	if (config->photonMapping)
		ans += PhotonMapper::photonColor(interceptP);
	if (config->causticMapping)
		ans += CausticMapper::photonColor(interceptP);
	for (vector<Light*>::const_iterator i = lights.begin(); i != lights.end(); i++) {
		if ((*i)->type == "PointLight") {
			if (selected->diffuseFactor + selected->specularFactor < EPS) continue;
			if (collide(((PointLight*)(*i))->position, interceptP)) continue;
			if (selected->diffuseFactor > EPS) {
				double d = dot(normalize(((PointLight*)(*i))->position - interceptP), normal);
				if (d > 0.0f)
					ans += selected->diffuseFactor * d * selected->getColor(interceptP) * ((PointLight*)(*i))->color;
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
		if ((*i)->type == "PlaneLight") {
			int totDiv = ((PlaneLight*)(*i))->xDiv * ((PlaneLight*)(*i))->yDiv;
			for (int j = 1; j <= ((PlaneLight*)(*i))->xDiv; j++)
				for (int k = 1; k <= ((PlaneLight*)(*i))->yDiv; k++) {
					Vector source = ((PlaneLight*)(*i))->getRandomPos(j, k);
					if (collide(source, interceptP)) continue;
					if (selected->diffuseFactor > EPS) {
						double d = dot(normalize(source - interceptP), normal);
						if (d > 0.0f)
							ans += selected->diffuseFactor * d * selected->getColor(interceptP) * ((PlaneLight*)(*i))->color / totDiv;
					}
					if (selected->specularFactor > EPS) {
						Vector reflectDirec = 2.0f * dot(-r.direction, normal) * normal + r.direction;
						double f = dot(normalize(source - interceptP), reflectDirec);
						if (f > 0.0f) {
							f = powF(f, selected->specularPower);
							ans += selected->specularFactor * f * ((PlaneLight*)(*i))->color / totDiv;
						}
					}
				}
		}
	}
	if (selected->reflectFactor > EPS) {
		// use diffuse reflection
		Vector reflectDirec = 2.0f * dot(-r.direction, normal) * normal + r.direction;
		Vector rn1 = Vector(reflectDirec.z, reflectDirec.y, -reflectDirec.x),
				rn2 = cross(reflectDirec, rn1);
		if ((co > 0.3f) && (selected->diffuseReflectValue > EPS)) { // only high co means discerniablity
			for (int i=0; i < config->diffuseReflectionTimes; ++i) {
				float xoffs, yoffs;
				do {
					xoffs = (double)rand() / (double)RAND_MAX * selected->diffuseReflectValue;
					yoffs = (double)rand() / (double)RAND_MAX * selected->diffuseReflectValue;
				} while ((xoffs * xoffs + yoffs * yoffs) > (selected->diffuseReflectValue * selected->diffuseReflectValue));
				Ray reflectRay(interceptP, normalize(reflectDirec + xoffs * rn1 + yoffs * rn2));
				ans += selected->reflectFactor / (double)config->diffuseReflectionTimes * work(reflectRay, co * selected->reflectFactor);
			}
		} else {
			Ray reflectRay(interceptP, normalize(reflectDirec));
			ans += selected->reflectFactor * work(reflectRay, co * selected->reflectFactor);
		}
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
	return regular(ans);
}

Color RayTracer::refractOutWork(const Ray &r, double co) {
	Vector totCrossPoint;
	Object *selected = nullptr;
	Light *crossLight = nullptr;
	int times = 0;
	Ray nowR = r;
	double dist = 0.0f;
	while (times <= 100) { //parameter's misery
		if (getCrossedObj(nowR, totCrossPoint, selected, crossLight) && (selected)) {
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
			// in the refraction body, no diffuce reflection is applied
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
	Object *selected = 0;
	Light *planeSelected = 0;

	Color ans = Color(0.0f, 0.0f, 0.0f);

	if (getCrossedObj(r, totCrossPoint, selected, planeSelected)) {
		// have intercept point
		if (selected)
			ans = objWork(r, totCrossPoint, co, selected);
		else
			ans = ((PlaneLight*)planeSelected)->color;
	} else {
		// calculate only light source
		for (vector<Light*>::iterator i = lights.begin(); i != lights.end(); i++) 
			if ((*i)->type != "PlaneLight")
				ans += (*i)->haloColor(r);
	}

	return ans;
}

bool RayTracer::getCrossedObj(const Ray &r, Vector& crossPoint, Object *(&crossObj), Light *(&crossLight)) {
	crossObj = nullptr;
	crossLight = nullptr;
	int crossType;
	double minDis = MAXINF;
	Vector nowCrossPoint;
	for (vector<Object*>::iterator i = objects.begin(); i != objects.end(); i++) {
		if ((*i)->intercept(r, nowCrossPoint)) {
			if ((getDistance2(r.origin, nowCrossPoint) < minDis) && (getDistance2(r.origin, nowCrossPoint) > EPS)) {
				minDis = getDistance2(r.origin, nowCrossPoint);
				crossObj = *i;
				crossPoint = nowCrossPoint;
				crossType = 0;
			}
		}
	}
	Object *tmpCrossObj;
	for (vector<Set*>::iterator s = sets.begin(); s != sets.end(); s++)
		if ((*s)->intercept(r, nowCrossPoint, tmpCrossObj)) {
			if ((getDistance2(r.origin, nowCrossPoint) < minDis) && (getDistance2(r.origin, nowCrossPoint) > EPS)) {
				minDis = getDistance2(r.origin, nowCrossPoint);
				crossObj = tmpCrossObj;
				crossPoint = nowCrossPoint;
				crossType = 0;
			}
		}
	
	for (vector<Light*>::iterator i = lights.begin(); i != lights.end(); i++)
		if ((*i)->type == "PlaneLight") 
			if (((PlaneLight*)(*i))->intercept(r, nowCrossPoint)) {
				if ((getDistance2(r.origin, nowCrossPoint) < minDis) && (getDistance2(r.origin, nowCrossPoint) > EPS)) {
					minDis = getDistance2(r.origin, nowCrossPoint);
					crossLight = *i;
					crossPoint = nowCrossPoint;
					crossType = 1;
				}
			}

	if ((crossObj == nullptr) && (crossLight == nullptr)) return false; else {
		if (crossType) crossObj = nullptr; else crossLight = nullptr;
		return true;
	}
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
	Object *obj;
	for (vector<Set*>::iterator s = sets.begin(); s != sets.end(); s++)
		if ((*s)->intercept(r, nowCrossPoint, obj)) {
			if ((getDistance2(S, nowCrossPoint) < minDis) && (getDistance2(S, nowCrossPoint) > EPS))
				minDis= getDistance2(r.origin, nowCrossPoint);
		}

	if ((minDis > EPS) && (getDistance2(S, T) - minDis > EPS)) 
		return true;
	else
		return false;
}

Config *RayTracer::getConfig() {
	return config;
}

Image *RayTracer::getImage() {
	return image;
}

vector<Light*> *RayTracer::getLights() {
	return &lights;
}

Camera *RayTracer::getCamera() {
	return camera;
}

vector<Object*> *RayTracer::getObjects() {
	return &objects;
}

vector<Set*> *RayTracer::getSets() {
	return &sets;
}