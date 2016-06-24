#include "PhotonMapper.h"
#include "Object.h"
#include "PlaneLight.h"
#include "PointLight.h"
#include "Set.h"

#define PI 3.1415926
#define MAXINF 1e+20
#define EPS 1e-6

Config *PhotonMapper::config = 0;
Image *PhotonMapper::image = 0;
vector<Light*> PhotonMapper::lights;
Camera *PhotonMapper::camera = 0;
vector<Object*> PhotonMapper::objects;
vector<Set*> PhotonMapper::sets;

Photon **PhotonMapper::photonArr = 0;
int PhotonMapper::totPhoton = 0;
PhotonNode *PhotonMapper::root = 0;

double PhotonMapper::R2 = 0.0f;

void PhotonMapper::q_sort(int L, int R, int direc, Photon **arr) {
	int i = L, j = R;
	Photon* t;
	double S;
	if (direc == 0) S = arr[(L+R)>>1]->origin.x;
	if (direc == 1) S = arr[(L+R)>>1]->origin.y;
	if (direc == 2) S = arr[(L+R)>>1]->origin.z;
	do {
		if (direc == 0) {
			while (arr[i]->origin.x < S - EPS) ++i;
			while (arr[j]->origin.x > S + EPS) --j;
		} else if (direc == 1) {
			while (arr[i]->origin.y < S - EPS) ++i;
			while (arr[j]->origin.y > S + EPS) --j;
		} else if (direc == 2) {
			while (arr[i]->origin.z < S - EPS) ++i;
			while (arr[j]->origin.z > S + EPS) --j;
		}
		if (i <= j) {
			t = arr[i], arr[i] = arr[j], arr[j] = t;
			++i, --j;
		}
	} while (i <= j);
	if (L < j) 
		q_sort(L, j, direc, arr);
	if (i < R)
		q_sort(i, R, direc, arr);
}

PhotonMapper::PhotonMapper() {
}

PhotonMapper::~PhotonMapper(void) {
}

void PhotonMapper::setConfig(Config *_conf) {
	config = _conf;
	R2 = config->photonR * config->photonR;
}

void PhotonMapper::setImage(Image* _image) {
	image = _image;
}

void PhotonMapper::setLights(vector<Light*> *_lights) {
	lights = *_lights;
}

void PhotonMapper::setCamera(Camera *_camera) {
	camera = _camera;
}

void PhotonMapper::setObjects(vector<Object*> *_objects) {
	objects = *_objects;
}

void PhotonMapper::setSets(vector<Set*> *_sets) {
	sets = *_sets;
}

void PhotonMapper::run() {
	if (config->photonMapping == 0) return;

	//photons generation
	thread *threadSet = new thread[config->totThread];
	vector<Photon*> *photonSets = new vector<Photon*>[config->totThread]();
	for (int i=0; i<config->totThread; i++) {
		threadSet[i] = thread(threadProc, i, rand(), photonSets + i);
	}
	for (int i=0; i<config->totThread; i++) {
		threadSet[i].join();
	}
	photonArr = new Photon*[lights.size() * config->photonTot];
	for (int j = 0; j < config->totThread; j++)
		for (vector<Photon*>::const_iterator i = photonSets[j].cbegin(); i != photonSets[j].cend(); i++)
			photonArr[totPhoton++] = *i;

	delete[] threadSet;
	delete[] photonSets;

	//KD-Tree generation
	cerr << "Total available photons: " << totPhoton << endl;
	cerr << "Build Photon KD-Trees Begin" << endl;
	root = new PhotonNode();
	root->e.Xmin = MAXINF, root->e.Ymin = MAXINF, root->e.Zmin = MAXINF;
	root->e.Xmax = -MAXINF, root->e.Ymax = -MAXINF, root->e.Zmax = -MAXINF;
	for (int i = 0; i < totPhoton; i++) {
		root->e += photonArr[i]->origin;
	}
	buildPhotonTree(root, 0, totPhoton-1, photonArr);
	cerr << "Build Photon KD-Trees Finish" << endl;
}

void PhotonMapper::threadProc(int s, int ran_seed, vector<Photon*> *threadSet) {
	srand(ran_seed);
	int upperBound = lights.size() * config->photonTot;
	for (; s < upperBound; s += config->totThread) {
		if (rand() < 0.01f * RAND_MAX) cerr<<s<<endl;
		int no = s / config->photonTot;
		Light *nowL = lights.at(no);
		if (nowL->type == "PointLight") {
			double t1 = double(rand()) / double(RAND_MAX), t2 = double(rand()) / double(RAND_MAX), t3 = double(rand()) / double(RAND_MAX);
			double theta = acos(1.0f - t2);
			double alpha = t3 * 2.0f * PI;
			if (t1 > 0.5f) theta += PI;
			Photon *p = new Photon(((PointLight*)nowL)->position, Vector(cos(theta), sin(theta) * sin(alpha), sin(theta) * cos(alpha)), ((PointLight*)nowL)->color);
			if (photonWork(p))
				threadSet->push_back(p);
		}
		if (nowL->type == "PlaneLight") {
			double t2 = double(rand()) / double(RAND_MAX), t3 = double(rand()) / double(RAND_MAX);
			//double theta = acos(1.0f - t2);
			//double theta = t2 * PI / 2.0f;
			double theta = PI / 2.0f * sqrt(t2);
			double alpha = t3 * 2.0f * PI;
			Vector x = ((PlaneLight*)nowL)->xVec, z = ((PlaneLight*)nowL)->N, y = cross(x, z);
			x = normalize(x), y = normalize(y);
			Vector orient = sin(theta) * sin(alpha) * x + sin(theta) * cos(alpha) * y + cos(theta) * z;
			Photon *p = new Photon(((PlaneLight*)nowL)->origin + double(rand()) / double(RAND_MAX) * ((PlaneLight*)nowL)->xVec + double(rand()) / double(RAND_MAX) * ((PlaneLight*)nowL)->yVec, orient, ((PlaneLight*)nowL)->color);
			if (photonWork(p)) {
				threadSet->push_back(p); 
			}
		}
	}
}

bool PhotonMapper::photonWork(Photon *p) {
	Vector totCrossPoint;
	Object *selected = 0;

	Color ans = Color(0.0f, 0.0f, 0.0f);

	Set *tmps;
	if (getCrossedObj(p, totCrossPoint, selected, tmps)) {
		return objWork(p, totCrossPoint, selected);
	} else
		return false;
}

bool PhotonMapper::getCrossedObj(Photon *p, Vector &crossPoint, Object *(&crossObj), Set *(&crossSet)) {
	crossObj = nullptr;
	crossSet = nullptr;
	double minDis = MAXINF;
	Vector nowCrossPoint;
	for (vector<Object*>::iterator i = objects.begin(); i != objects.end(); i++) {
		if ((*i)->intercept(*p, nowCrossPoint)) {
			if ((getDistance2(p->origin, nowCrossPoint) < minDis) && (getDistance2(p->origin, nowCrossPoint) > EPS)) {
				minDis = getDistance2(p->origin, nowCrossPoint);
				crossObj = *i;
				crossPoint = nowCrossPoint;
			}
		}
	}
	Object *tmpCrossObj;
	for (vector<Set*>::iterator s = sets.begin(); s != sets.end(); s++)
		if ((*s)->intercept(*p, nowCrossPoint, tmpCrossObj)) {
			if ((getDistance2(p->origin, nowCrossPoint) < minDis) && (getDistance2(p->origin, nowCrossPoint) > EPS)) {
				minDis = getDistance2(p->origin, nowCrossPoint);
				crossObj = tmpCrossObj;
				crossSet = *s;
				crossPoint = nowCrossPoint;
			}
		}
	
	if (crossObj == nullptr) return false; else return true;
}

bool PhotonMapper::objWork(Photon *p, Vector interceptP, Object *selected) {
	Vector normal;
	selected->getNormal(interceptP, normal);

	double lucky = (double)rand() / (double)RAND_MAX;

	if (lucky <= (selected->specularFactor + selected->reflectFactor)) {
		// use specular reflection
		Vector reflectDirec = 2.0f * dot(-p->direction, normal) * normal + p->direction;
		Vector rn1 = Vector(reflectDirec.z, reflectDirec.y, -reflectDirec.x),
				rn2 = cross(reflectDirec, rn1);
		if (selected->diffuseReflectValue > EPS) { // only high co means discerniablity
			float xoffs, yoffs;
			do {
				xoffs = (double)rand() / (double)RAND_MAX * selected->diffuseReflectValue;
				yoffs = (double)rand() / (double)RAND_MAX * selected->diffuseReflectValue;
			} while ((xoffs * xoffs + yoffs * yoffs) > (selected->diffuseReflectValue * selected->diffuseReflectValue));
			Ray reflectRay(interceptP, normalize(reflectDirec + xoffs * rn1 + yoffs * rn2));
			p->origin = interceptP;
			p->direction = normalize(reflectDirec + xoffs * rn1 + yoffs * rn2);
		} else {
			p->origin = interceptP;
			p->direction = normalize(reflectDirec);
		}
		return photonWork(p);
	} else if (lucky <= (selected->specularFactor + selected->reflectFactor + selected->diffuseFactor)) {
		// use diffuse reflection
		double t2 = double(rand()) / double(RAND_MAX), t3 = double(rand()) / double(RAND_MAX);
		double theta = acos(1.0f - t2);
		double alpha = t3 * 2.0f * PI;
		Vector x = getArbitraryPlaneVec(normal), z = normal, y = getPerpendicularVec(z, x);
		Vector difRec = sin(theta) * sin(alpha) * x + sin(theta) * cos(alpha) * y + cos(theta) * z;
		p->origin = interceptP;
		p->direction = difRec;
		p->color = p->color * selected->getColor(interceptP);
		return photonWork(p);
	} else if (lucky <= (selected->specularFactor + selected->reflectFactor + selected->diffuseFactor + selected->refractFactor)) {
		// use refract reflection

		double nInv = 1.0f / selected->refractN;
		double cosI = -dot(normal, p->direction);
		if (cosI > 0) {
			double cosT2 = 1.0f - nInv * nInv * (1.0f - cosI * cosI);
			if (cosT2 > 0.0f) {
				Vector T = (nInv * p->direction) + (nInv * cosI - sqrt(cosT2)) * normal;
				p->origin = interceptP + T * EPS;
				p->direction = T;
				p->color = p->color * selected->getColor(interceptP);
				Ray refractRay(interceptP + T * EPS, T);
				return refractOutWork(p, selected);
			}
		} else {
			double cosT2 = 1.0f - selected->refractN * selected->refractN * (1.0f - cosI * cosI);
			if (cosT2 > 0.0f) {
				Vector T = (selected->refractN * p->direction) + (sqrt(cosT2) + selected->refractN * cosI) * normal;
				p->origin = interceptP + T * EPS;
				p->direction = T;
				p->color = p->color * selected->getColor(interceptP);
				Ray refractRay(interceptP + T * EPS, T);
				return photonWork(p);
			}
		}

		/*
		double nInv = 1.0f / selected->refractN;
		double cosI = -dot(normal, p->direction);
		if (cosI < -EPS) 
			cosI = -cosI, nInv = selected->refractN;
		double cosT2 = 1.0f - nInv * nInv * (1.0f - cosI * cosI);
		if (cosT2 > 0.0f) {
			Vector T = (nInv * p->direction) + (nInv * cosI - sqrt(cosT2)) * normal;
			p->origin = interceptP + T * EPS;
			p->direction = T;
			p->color = p->color * selected->getColor(interceptP);
			Ray refractRay(interceptP + T * EPS, T);
			return refractOutWork(p);
		} else {
			return false; // cannot refract in, discard
		}
		*/
	} else { 
		// absorbed
		Vector normal;
		selected->getNormal(interceptP, normal);
		//p->color *= -dot(p->direction, normal);
		p->origin = interceptP;
		return true;
	}
}

bool PhotonMapper::refractOutWork(Photon *p, Object *from) {
	Vector totCrossPoint;
	Object *selected = nullptr;
	int times = 0;
	Photon *nowR = p;
	Set *tmps;
	while (times <= 10) { //parameter's misery
		if (getCrossedObj(p, totCrossPoint, selected, tmps)) {
			Vector normal;
			selected->getNormal(totCrossPoint, normal);
			double nInv = 1.0f / selected->refractN;
			double cosI = -dot(normal, nowR->direction);
			if (selected != from) { // trick: when Plane as bottom, directly use it
				return objWork(nowR, totCrossPoint, selected);
			}
			if (cosI > 0) {
				double cosT2 = 1.0f - nInv * nInv * (1.0f - cosI * cosI);
				if (cosT2 > 0.0f) {
					Vector T = (nInv * nowR->direction) + (nInv * cosI - sqrt(cosT2)) * normal;
					nowR->origin = totCrossPoint + T * EPS;
					nowR->direction = T;
					return photonWork(nowR);
				}
			} else {
				double cosT2 = 1.0f - selected->refractN * selected->refractN * (1.0f - cosI * cosI);
				if (cosT2 > 0.0f) {
					Vector T = (selected->refractN * nowR->direction) + (sqrt(cosT2) + selected->refractN * cosI) * normal;
					nowR->origin = totCrossPoint + T * EPS;
					nowR->direction = T;
					return photonWork(nowR);
				}
			}
			++times;
			// in the refraction body, no diffuce reflection is applied
			Vector reflectDirec = 2.0f * dot(-nowR->direction, normal) * normal + nowR->direction;
			nowR->origin = totCrossPoint + reflectDirec * EPS;
			nowR->direction = reflectDirec;
			return photonWork(nowR);
		} else 
			return false;
	}
	return false;
}

void PhotonMapper::buildPhotonTree(PhotonNode *p, int l, int r, Photon** photonArr) {
	if (r-l <= 2) {
		p->direc = 3;
		for (int i=l; i<=r; i++)
			p->photons.push_back(photonArr[i]);
		return;
	} else {
		double maxD = 0.0f, direc = -1;
		double Xmin = MAXINF, Ymin = MAXINF, Zmin = MAXINF, Xmax = -MAXINF, Ymax = -MAXINF, Zmax = -MAXINF;
		for (int i=l; i<=r; i++) {
			if (photonArr[i]->origin.x < Xmin) Xmin = photonArr[i]->origin.x;
			if (photonArr[i]->origin.y < Ymin) Ymin = photonArr[i]->origin.y;
			if (photonArr[i]->origin.z < Zmin) Zmin = photonArr[i]->origin.z;
			if (photonArr[i]->origin.x > Xmax) Xmax = photonArr[i]->origin.x;
			if (photonArr[i]->origin.y > Ymax) Ymax = photonArr[i]->origin.y;
			if (photonArr[i]->origin.z > Zmax) Zmax = photonArr[i]->origin.z;
		}
		if (Xmax - Xmin > maxD)
			maxD = Xmax - Xmin, direc = 0;
		if (Ymax - Ymin > maxD)
			maxD = Ymax - Ymin, direc = 1;
		if (Zmax - Zmin > maxD)
			maxD = Zmax - Zmin, direc = 2;
		if (maxD < EPS) {
			for (int i=l; i<=r; i++)
				p->photons.push_back(photonArr[i]);
			p->direc = 3;
			return;
		}

		q_sort(l, r, direc, photonArr);
		int mid = (l+r)>>1;
		while (mid < r) {
			if (direc == 0)
				if (photonArr[mid + 1]->origin.x - photonArr[mid]->origin.x <= EPS) ++mid; else break;
			if (direc == 1)
				if (photonArr[mid + 1]->origin.y - photonArr[mid]->origin.y <= EPS) ++mid; else break;
			if (direc == 2)
				if (photonArr[mid + 1]->origin.z - photonArr[mid]->origin.z <= EPS) ++mid; else break;
		}
		if (mid == r) {
			for (int i=l; i<=r; i++)
				p->photons.push_back(photonArr[i]);
			p->direc = 3;
			return;
		}
		p->direc = direc;
		p->left = new PhotonNode();
		p->right = new PhotonNode();
		p->left->parent = p;
		p->right->parent = p;
		p->left->e = p->e;
		p->right->e = p->e;
		if (direc == 0) {
			p->left->e.Xmax = p->right->e.Xmin = (photonArr[mid]->origin.x + photonArr[mid+1]->origin.x)/2.0f;
			p->left->border = photonArr[mid]->origin.x;
			p->right->border = photonArr[mid+1]->origin.x;
		}
		if (direc == 1) {
			p->left->e.Ymax = p->right->e.Ymin = (photonArr[mid]->origin.y + photonArr[mid+1]->origin.y)/2.0f;
			p->left->border = photonArr[mid]->origin.y;
			p->right->border = photonArr[mid+1]->origin.y;
		}
		if (direc == 2) {
			p->left->e.Zmax = p->right->e.Zmin = (photonArr[mid]->origin.z + photonArr[mid+1]->origin.z)/2.0f;
			p->left->border = photonArr[mid]->origin.z;
			p->right->border = photonArr[mid+1]->origin.z;
		}
		buildPhotonTree(p->left, l, mid, photonArr);
		buildPhotonTree(p->right, mid+1, r, photonArr);
	}
}

Color PhotonMapper::photonColor(const Vector &p) {
	if (config->photonMapping == 0) return Color();
	priority_queue<SimplePhoton> *selected = new priority_queue<SimplePhoton>();

	traverseTree(p, root, selected);
	Color now = Color(0.0f, 0.0f, 0.0f);
	double r = R2;
	if (selected->size() == config->photonN) 
		r = selected->top().len;
	
	while (!selected->empty()) {
		now += selected->top().c;
		selected->pop();
	}
	
	now *= config->photonLuminosity;
	now /= r;
	delete selected;
	return now;
}

void PhotonMapper::traverseTree(const Vector &p, const PhotonNode *now, priority_queue<SimplePhoton> *selected) {
	if (now->direc != 3) {
		double r = config->photonR;
		if (selected->size() == config->photonN)
			r = sqrt(selected->top().len);
		if (now->direc == 0) {
			if (p.x <= now->left->e.Xmax) {
				traverseTree(p, now->left, selected);
				if (now->right->border - p.x < r)
					traverseTree(p, now->right, selected);
			} else {
				traverseTree(p, now->right, selected);
				if (p.x - now->left->border < r)
					traverseTree(p, now->left, selected);
			}
		} else if (now->direc == 1) {
			if (p.y <= now->left->e.Ymax) {
				traverseTree(p, now->left, selected);
				if (now->right->border - p.y < r)
					traverseTree(p, now->right, selected);
			} else {
				traverseTree(p, now->right, selected);
				if (p.y - now->left->border < r)
					traverseTree(p, now->left, selected);
			}
		} else if (now->direc == 2) {
			if (p.z <= now->left->e.Zmax) {
				traverseTree(p, now->left, selected);
				if (now->right->border - p.z < r)
					traverseTree(p, now->right, selected);
			} else {
				traverseTree(p, now->right, selected);
				if (p.z - now->left->border < r)
					traverseTree(p, now->left, selected);
			}
		}
	} else {
		for (vector<Photon*>::const_iterator i = now->photons.cbegin(); i != now->photons.cend(); i++)
			if (selected->size() == config->photonN) {
				if (getDistance2((*i)->origin, p) < selected->top().len) {
					selected->pop();
					selected->push(SimplePhoton(**i, p));
				}
			} else if (getDistance2((*i)->origin, p) < R2)
				selected->push(SimplePhoton(**i, p));
	}
}
