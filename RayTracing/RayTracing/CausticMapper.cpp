#include "CausticMapper.h"
#include "Object.h"
#include "PlaneLight.h"
#include "PointLight.h"
#include "Set.h"
#include "Sphere.h"

#define PI 31.1415926
#define MAXINF 1e+20
#define EPS 1e-6

Photon **CausticMapper::photonArr = 0;
int CausticMapper::totPhoton = 0;
PhotonNode *CausticMapper::root = 0;

double CausticMapper::R2 = 0.0f;

CausticMapper::CausticMapper() {
}

CausticMapper::~CausticMapper() {
}

void CausticMapper::setR(Config* _conf) {
	R2 = _conf->causticR * _conf->causticR;
}

void CausticMapper::run() {
	if (config->causticMapping == 0) return;

	// find the work need to do
	int totThread = 0;
	for (vector<Object*>::iterator i = PhotonMapper::objects.begin(); i != PhotonMapper::objects.end(); i++) {
		if (((*i)->type == "Sphere") && (((Sphere*)*i)->causticTot))
			++totThread;
	}
	for (vector<Set*>::iterator i = PhotonMapper::sets.begin(); i != PhotonMapper::sets.end(); i++) {
		if ((*i)->causticTot)
			++totThread;
	}
	totThread *= PhotonMapper::lights.size();
	int p = 0;
	int *lightNo = new int[totThread], *aimNo = new int[totThread], *totArr = new int[totThread];
	for (int i = 0; i < PhotonMapper::lights.size(); ++i) {
		for (int j = 0; j < PhotonMapper::objects.size(); ++j) {
			if ((PhotonMapper::objects[j]->type == "Sphere") && (((Sphere*)PhotonMapper::objects[j])->causticTot)) {
				lightNo[p] = i, aimNo[p] = (j << 1), totArr[p] = ((Sphere*)PhotonMapper::objects[j])->causticTot;
				++p;
				// sphere is even no
			}
		}
		for (int j = 0; j < PhotonMapper::sets.size(); ++j) {
			if (PhotonMapper::sets[j]->causticTot) {
				lightNo[p] = i, aimNo[p] = (j << 1) + 1, totArr[p] = PhotonMapper::sets[j]->causticTot;
				++p;
				// set is odd no
			}
		}
	}

	//photons generation
	thread *threadSet = new thread[totThread];
	vector<Photon*> *photonSets = new vector<Photon*>[totThread];
	for (int i=0; i<totThread; i++) {
		threadSet[i] = thread(threadProc, lightNo[i], aimNo[i], totArr[i], photonSets + i, rand());
	}
	for (int i=0; i<totThread; i++) {
		threadSet[i].join();
	}

	int totPhotonMax = 0;
	for (int i=0; i<totThread; i++)
		totPhotonMax += totArr[i];
	photonArr = new Photon*[totPhotonMax];
	for (int j=0; j<totThread; j++)
		for (vector<Photon*>::const_iterator i = photonSets[j].cbegin(); i != photonSets[j].cend(); i++)
			photonArr[totPhoton++] = *i;

	delete[] lightNo;
	delete[] aimNo;
	delete[] totArr;
	delete[] threadSet;
	delete[] photonSets;

	//KD-Tree generation
	cerr << "Total available caustic photons: " << totPhoton << endl;
	cerr << "Build Caustic Photon KD-Trees Begin" << endl;
	root = new PhotonNode();
	root->e.Xmin = MAXINF, root->e.Ymin = MAXINF, root->e.Zmin = MAXINF;
	root->e.Xmax = -MAXINF, root->e.Ymax = -MAXINF, root->e.Zmax = -MAXINF;
	for (int i=0; i < totPhoton; i++) {
		root->e += photonArr[i]->origin;
	}
	buildPhotonTree(root, 0, totPhoton-1, CausticMapper::photonArr);
	cerr << "Build Caustic Photon KD-Trees Finish" << endl;
}

void CausticMapper::threadProc(int lightNo, int aimNo, int tot, vector<Photon*> *threadSet, int ran_seed) {
	srand(ran_seed);
	int nowCnt = 0, totTimes = 0;
	for (; nowCnt < tot;) {
		if ((nowCnt == 0) && (totTimes > tot)) break;
		if (rand() < 0.01f * RAND_MAX) 
			cerr << lightNo << " " << aimNo << " " << nowCnt << " " << tot << " " 
			<< totTimes << endl;
		Photon *origin;
		Light *nowL = lights.at(lightNo);
		if (nowL->type == "PointLight") {
			double t1 = double(rand()) / double(RAND_MAX), t2 = double(rand()) / double(RAND_MAX), t3 = double(rand()) / double(RAND_MAX);
			double theta = acos(1.0f - t2);
			double alpha = t3 * 2.0f * PI;
			if (t1 > 0.5f) theta += PI;
			origin = new Photon(((PointLight*)nowL)->position, Vector(cos(theta), sin(theta) * sin(alpha), sin(theta) * cos(alpha)), ((PointLight*)nowL)->color);
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
			origin = new Photon(((PlaneLight*)nowL)->origin + double(rand()) / double(RAND_MAX) * ((PlaneLight*)nowL)->xVec + double(rand()) / double(RAND_MAX) * ((PlaneLight*)nowL)->yVec, orient, ((PlaneLight*)nowL)->color);
		}
		Object *now;
		Set *nows;
		Vector tmpPoint;
		getCrossedObj(origin, tmpPoint, now, nows);
		if (((aimNo & 1) && (nows == PhotonMapper::sets[aimNo >> 1])) || 
			(((aimNo & 1) == 0) && (now == PhotonMapper::objects[aimNo >> 1]))) {
			++nowCnt;
			if (photonWork(origin)) {
				threadSet->push_back(origin);
			}
		}
		++totTimes;
	}
}

bool CausticMapper::photonWork(Photon *p) {
	Vector totCrossPoint;
	Object *selected = 0;

	Color ans = Color(0.0f, 0.0f, 0.0f);

	Set *tmps;
	if (getCrossedObj(p, totCrossPoint, selected, tmps)) {
		return objWork(p, totCrossPoint, selected);
	} else
		return false;
}

bool CausticMapper::objWork(Photon *p, Vector interceptP, Object* selected) {
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
	} else if (selected->refractFactor > EPS) {
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

bool CausticMapper::refractOutWork(Photon *p, Object *from) {
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

Color CausticMapper::photonColor(const Vector &p) {
	if (config->causticMapping == 0) return Color();
	priority_queue<SimplePhoton> *selected = new priority_queue<SimplePhoton>();

	traverseTree(p, root, selected);
	Color now = Color(0.0f, 0.0f, 0.0f);
	double r = R2;
	if (selected->size() == config->causticN) 
		r = selected->top().len;
	
	while (!selected->empty()) {
		double coef = 1.0f - sqrt(selected->top().len) / (config->causticK * config->causticR);
		if (coef > EPS)
			now += coef * selected->top().c;
		selected->pop();
	}
	
	now *= config->causticLuminosity;
	now /= r;
	now /= (1.0f - 2.0f / (config->causticK * 3.0f));
	delete selected;
	return now;
}

void CausticMapper::traverseTree(const Vector &p, const PhotonNode *now, priority_queue<SimplePhoton> *selected) {
	if (now->direc != 3) {
		double r = config->causticR;
		if (selected->size() == config->causticN)
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
			if (selected->size() == config->causticN) {
				if (getDistance2((*i)->origin, p) < selected->top().len) {
					selected->pop();
					selected->push(SimplePhoton(**i, p));
				}
			} else if (getDistance2((*i)->origin, p) < R2)
				selected->push(SimplePhoton(**i, p));
	}
}
