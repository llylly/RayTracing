#include "KDBuilder.h"
#include "KDNode.h"

#define EPS 1e-6

void q_sort(double *a, int *b, int L, int R) {
	int i = L, j = R, tt;
	double x = a[(L + R) >> 1], t;
	do {
		while (a[i] < x - EPS) ++i;
		while (a[j] > x + EPS) --j;
		if (i <= j) {
			t = a[i], a[i] = a[j], a[j] = t;
			tt = b[i], b[i] = b[j], b[j] = tt;
			++i, --j;
		}
	} while (i <= j);
	if (L < j) 
		q_sort(a, b, L, j);
	if (i < R)
		q_sort(a, b, i, R);
}

const double KDBuilder::C_traversal = 1.0f;
const double KDBuilder::C_intersect = 2.0f;

KDBuilder::KDBuilder() {
}


KDBuilder::~KDBuilder() {
}

bool KDBuilder::work(KDNode *parent) {
	if ((parent->objs.size() <= 2) || (parent->depth >= 20)) return false;

	double pV = parent->box.area() * parent->objs.size() * C_intersect;
	double *xp = new double[parent->objs.size() << 1];
	int *xflag = new int[parent->objs.size() << 1];
	double *yp = new double[parent->objs.size() << 1];
	int *yflag = new int[parent->objs.size() << 1];
	double *zp = new double[parent->objs.size() << 1];
	int *zflag = new int[parent->objs.size() << 1];
	for (int i = 0; i < parent->objs.size(); ++i) {
		xp[i << 1] = (parent->objs)[i]->box->Xmin;
		xflag[i << 1] = 1;
		yp[i << 1] = (parent->objs)[i]->box->Ymin;
		yflag[i << 1] = 1;
		zp[i << 1] = (parent->objs)[i]->box->Zmin;
		zflag[i << 1] = 1;
		xp[(i << 1) + 1] = (parent->objs)[i]->box->Xmax;
		xflag[(i << 1) + 1] = -1;
		yp[(i << 1) + 1] = (parent->objs)[i]->box->Ymax;
		yflag[(i << 1) + 1] = -1;
		zp[(i << 1) + 1] = (parent->objs)[i]->box->Zmax;
		zflag[(i << 1) + 1] = -1;
	}
	q_sort(xp, xflag, 0, (parent->objs.size() << 1) - 1);
	q_sort(yp, yflag, 0, (parent->objs.size() << 1) - 1);
	q_sort(zp, zflag, 0, (parent->objs.size() << 1) - 1);

	double minCost = pV;
	double deviLine;
	int deviType = 3;
	
	double area = parent->box.area();

	double nowCost, nowLine;
	int now0, now1, nowsum;

	if (xp[(parent->objs.size() << 1) - 1] - xp[0] > EPS) {
		double sliceA = parent->box.area() / (parent->box.Xmax - parent->box.Xmin);
		now0 = now1 = nowsum = 0;
		nowsum += xflag[0];
		if (xflag[0] == -1) ++now0;
		if (xflag[0] == 1) ++now1;
		for (int i = 0; i < (parent->objs.size() << 1); ) {
			while ((i < ((parent->objs.size() << 1) - 1)) && (xp[i+1] - xp[i] < EPS)) {
				++i;
				nowsum += xflag[i];
				if (xflag[i] == -1) ++now0;
				if (xflag[i] == 1) ++now1;
			}
			if (i == ((parent->objs.size() << 1) - 1)) 
				nowLine = parent->box.Xmax;
			else
				nowLine = (xp[i] + xp[i+1]) / 2.0f;
			nowCost = C_traversal * area +
				sliceA * (nowLine - parent->box.Xmin) * (now0 + nowsum) * C_intersect +
				sliceA * (parent->box.Xmax - nowLine) * (parent->objs.size() - now1 + nowsum) * C_intersect;
			if (nowCost < minCost) {
				deviType = 0;
				deviLine = nowLine;
				minCost = nowCost;
			}
			++i;
			nowsum += xflag[i];
			if (xflag[i] == -1) ++now0;
			if (xflag[i] == 1) ++now1;
		}
	}

	if (yp[(parent->objs.size() << 1) - 1] - yp[0] > EPS) {
		double sliceA = parent->box.area() / (parent->box.Ymax - parent->box.Ymin);
		now0 = now1 = nowsum = 0;
		nowsum += yflag[0];
		if (yflag[0] == -1) ++now0;
		if (yflag[0] == 1) ++now1;
		for (int i = 0; i < (parent->objs.size() << 1); ) {
			while ((i < ((parent->objs.size() << 1) - 1)) && (yp[i+1] - yp[i] < EPS)) {
				++i;
				nowsum += yflag[i];
				if (yflag[i] == -1) ++now0;
				if (yflag[i] == 1) ++now1;
			}
			if (i == ((parent->objs.size() << 1) - 1)) 
				nowLine = parent->box.Ymax;
			else
				nowLine = (yp[i] + yp[i+1]) / 2.0f;
			nowCost = C_traversal * area +
				sliceA * (nowLine - parent->box.Ymin) * (now0 + nowsum) * C_intersect +
				sliceA * (parent->box.Ymax - nowLine) * (parent->objs.size() - now1 + nowsum) * C_intersect;
			if (nowCost < minCost) {
				deviType = 1;
				deviLine = nowLine;
				minCost = nowCost;
			}
			++i;
			nowsum += yflag[i];
			if (yflag[i] == -1) ++now0;
			if (yflag[i] == 1) ++now1;
		}
	}

	if (zp[(parent->objs.size() << 1) - 1] - zp[0] > EPS) {
		double sliceA = parent->box.area() / (parent->box.Zmax - parent->box.Zmin);
		now0 = now1 = nowsum = 0;
		nowsum = zflag[0];
		if (zflag[0] == -1) ++now0;
		if (zflag[0] == 1) ++now1;
		for (int i = 0; i < (parent->objs.size() << 1); ) {
			while ((i < ((parent->objs.size() << 1) - 1)) && (zp[i+1] - zp[i] < EPS)) {
				++i;
				nowsum += zflag[i];
				if (zflag[i] == -1) ++now0;
				if (zflag[i] == 1) ++now1;
			}
			if (i == ((parent->objs.size() << 1) - 1)) 
				nowLine = parent->box.Zmax;
			else
				nowLine = (zp[i] + zp[i+1]) / 2.0f;
			nowCost = C_traversal * area +
				sliceA * (nowLine - parent->box.Zmin) * (now0 + nowsum) * C_intersect +
				sliceA * (parent->box.Zmax - nowLine) * (parent->objs.size() - now1 + nowsum) * C_intersect;
			if (nowCost < minCost) {
				deviType = 2;
				deviLine = nowLine;
				minCost = nowCost;
			}
			++i;
			nowsum += zflag[i];
			if (zflag[i] == -1) ++now0;
			if (zflag[i] == 1) ++now1;
		}
	}

	delete[] xflag;
	delete[] yflag;
	delete[] zflag;
	delete[] xp;
	delete[] yp;
	delete[] zp;

	if (deviType != 3) {
		Enclosure lBox = parent->box, rBox = parent->box;
		parent->direc = deviType;
		if (deviType == 0) {
			lBox.Xmax = deviLine;
			rBox.Xmin = deviLine;
		}
		if (deviType == 1) {
			lBox.Ymax = deviLine;
			rBox.Ymin = deviLine;
		}
		if (deviType == 2) {
			lBox.Zmax = deviLine;
			rBox.Zmin = deviLine;
		}
		parent->left = new KDNode(lBox, 3, parent->depth + 1);
		parent->left->father = parent;
		parent->right = new KDNode(rBox, 3, parent->depth + 1);
		parent->right->father = parent;

		for (vector<Mesh*>::iterator i = parent->objs.begin(); i != parent->objs.end(); i++) {
			bool inL = false, inR = false;
			if (deviType == 0) {
				if ((*i)->box->Xmin <= deviLine) inL = true;
				if ((*i)->box->Xmax >= (deviLine + EPS)) inR = true;
			}
			if (deviType == 1) {
				if ((*i)->box->Ymin <= deviLine) inL = true;
				if ((*i)->box->Ymax >= (deviLine + EPS)) inR = true;
			}
			if (deviType == 2) {
				if ((*i)->box->Zmin <= deviLine) inL = true;
				if ((*i)->box->Zmax >= (deviLine + EPS)) inR = true;
			}
			if (inL) parent->left->addMesh(*i);
			if (inR) parent->right->addMesh(*i);
		}
		parent->objs.clear();

		return true;
	}

	return false;
}