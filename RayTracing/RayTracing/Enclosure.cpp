#include "Enclosure.h"

#define INF 1e+20
#define EPS 1e-6

Enclosure::Enclosure() {
	Xmin = Xmax = Ymin = Ymax = Zmin = Zmax = 0.0f;
}

Enclosure::Enclosure(double _xmin, double _xmax, double _ymin, double _ymax, double _zmin, double _zmax) {
	Xmin = _xmin, Xmax = _xmax,
	Ymin = _ymin, Ymax = _ymax,
	Zmin = _zmin, Zmax = _zmax;
}

Enclosure::~Enclosure() {
}

bool Enclosure::inBox(const Enclosure &e, const Vector &v) {
	return (v.x >= e.Xmin) & (v.x < e.Xmax) & (v.y >= e.Ymin) & (v.y < e.Ymax) & (v.z >= e.Zmin) & (v.z < e.Zmax);
}

Enclosure &Enclosure::operator+=(const Enclosure &b) {
	this->Xmin = (b.Xmin < this->Xmin)? b.Xmin : this->Xmin;
	this->Ymin = (b.Ymin < this->Ymin)? b.Ymin : this->Ymin;
	this->Zmin = (b.Zmin < this->Zmin)? b.Zmin : this->Zmin;
	this->Xmax = (b.Xmax > this->Xmax)? b.Xmax : this->Xmax;
	this->Ymax = (b.Ymax > this->Ymax)? b.Ymax : this->Ymax;
	this->Zmax = (b.Zmax > this->Zmax)? b.Zmax : this->Zmax;
	return *this;
}

Enclosure &Enclosure::operator+=(const Vector &b) {
	this->Xmin = (b.x < this->Xmin)? b.x : this->Xmin;
	this->Ymin = (b.y < this->Ymin)? b.y : this->Ymin;
	this->Zmin = (b.z < this->Zmin)? b.z : this->Zmin;
	this->Xmax = (b.x > this->Xmax)? b.x : this->Xmax;
	this->Ymax = (b.y > this->Ymax)? b.y : this->Ymax;
	this->Zmax = (b.z > this->Zmax)? b.z : this->Zmax;
	return *this;
}

Enclosure operator+(const Enclosure &a, const Enclosure &b) {
	Enclosure n(min(a.Xmin, b.Xmin), max(a.Xmax, b.Xmax), min(a.Ymin, b.Ymin), max(a.Ymax, b.Ymax), min(a.Zmin, b.Zmin), max(a.Zmax, b.Zmax));
	return n;
}

double Enclosure::area() {
	return (Xmax - Xmin) * (Ymax - Ymin) * (Zmax - Zmin);
}

bool Enclosure::intercept(const Enclosure &e, const Ray &r, Vector &p) {
	double minD = INF, nowD;
	bool flag = false;
	Vector nowp;
	if (abs(r.direction.x) > EPS) {
		nowD = (e.Xmin - r.origin.x) / r.direction.x;
		if (nowD > 0.0f) {
			nowp = r.origin + nowD * r.direction;
			if (nowD < minD)
				if ((nowp.y >= e.Ymin) && (nowp.y <= e.Ymax) && (nowp.z >= e.Zmin) && (nowp.z <= e.Zmax))
					flag = true, minD = nowD;
		}
		nowD = (e.Xmax - r.origin.x) / r.direction.x;
		if (nowD > 0.0f) {
			nowp = r.origin + nowD * r.direction;
			if (nowD < minD)
				if ((nowp.y >= e.Ymin) && (nowp.y <= e.Ymax) && (nowp.z >= e.Zmin) && (nowp.z <= e.Zmax))
					flag = true, minD = nowD;
		}
	}
	if (abs(r.direction.y) > EPS) {
		nowD = (e.Ymin - r.origin.y) / r.direction.y;
		if (nowD > 0.0f) {
			nowp = r.origin + nowD * r.direction;
			if (nowD < minD)
				if ((nowp.x >= e.Xmin) && (nowp.x <= e.Xmax) && (nowp.z >= e.Zmin) && (nowp.z <= e.Zmax))
					flag = true, minD = nowD;
		}
		nowD = (e.Ymax - r.origin.y) / r.direction.y;
		if (nowD > 0.0f) {
			nowp = r.origin + nowD * r.direction;
			if (nowD < minD)
				if ((nowp.x >= e.Xmin) && (nowp.x <= e.Xmax) && (nowp.z >= e.Zmin) && (nowp.z <= e.Zmax))
					flag = true, minD = nowD;
		}
	}
	if (abs(r.direction.z) > EPS) {
		nowD = (e.Zmin - r.origin.z) / r.direction.z;
		if (nowD > 0.0f) {
			nowp = r.origin + nowD * r.direction;
			if (nowD < minD)
				if ((nowp.x >= e.Xmin) && (nowp.x <= e.Xmax) && (nowp.y >= e.Ymin) && (nowp.y <= e.Ymax))
					flag = true, minD = nowD;
		}
		nowD = (e.Zmax - r.origin.z) / r.direction.z;
		if (nowD > 0.0f) {
			nowp = r.origin + nowD * r.direction;
			if (nowD < minD)
				if ((nowp.x >= e.Xmin) && (nowp.x <= e.Xmax) && (nowp.y >= e.Ymin) && (nowp.y <= e.Ymax))
					flag = true, minD = nowD;
		}
	}
	if (flag) p = r.origin + minD * r.direction;
	return flag;
}

double Enclosure::minDis2(const Vector &p, const Enclosure &e) {
	Vector minP;
	if (p.x >= e.Xmax) minP.x = e.Xmax;
	else if (p.x <= e.Xmin) minP.x = e.Xmin;
	else minP.x = p.x;
	if (p.y >= e.Ymax) minP.y = e.Ymax;
	else if (p.y <= e.Ymin) minP.y = e.Ymin;
	else minP.y = p.y;
	if (p.z >= e.Zmax) minP.z = e.Zmax;
	else if (p.z <= e.Zmin) minP.z = e.Zmin;
	else minP.z = p.z;
	return getDistance2(minP, p);
}