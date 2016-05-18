#define EPS 1e-6
#include "Mesh.h"

Mesh::Mesh() {
	pointTot = 0;
	points = new vector<Vector>;
	type = "Mesh";
}

Mesh::Mesh(Vector _normal, vector<Vector> *_points, int _tot, Color _color, double _diffuseFactor, double _specularFactor, int _specularPower,
		double _reflectFactor, double _environmentFactor, double _refractFactor, double _refractN, double _beerConst, bool _recalNormal) {
	pointTot = _tot;
	points = new vector<Vector>;
	type = "Mesh";

	for (vector<Vector>::const_iterator i = _points->begin(); i != _points->end(); i++) {
		points->push_back(*i);
	}
	type = "Mesh";
	position = points->at(0);
	bgColor = _color, diffuseFactor = _diffuseFactor, specularFactor = _specularFactor, specularPower = _specularPower,
				reflectFactor = _reflectFactor, environmentFactor = _environmentFactor,
				refractFactor = _refractFactor, refractN = _refractN;
	normal = cross(points->at(1) - points->at(0), points->at(2) - points->at(0));
	normal = normalize(normal * dot(normal, _normal));
	if ((pointTot >= 3) && (_recalNormal)) {
		N = normal;
	} else 
		N = _normal;
}

Mesh::~Mesh() {
	delete points;
}

bool Mesh::intercept(const Ray &r, Vector &p) {
	if (abs(dot(normal, r.direction))<EPS) return false;
	double T = dot(normal, position - r.origin) / dot(N, r.direction);
	if (T < 0.0f) return false;
	p = r.origin + T * r.direction;

	int cnt = 0;
	double x, y;
	Vector R;
	if (abs(normal.z) > EPS) {
		R.x = (double)rand() / (double)RAND_MAX * 1000.0f;
		R.y = (double)rand() / (double)RAND_MAX * 1000.0f;
		R.z = -(normal.x * R.x + normal.y * R.y) / normal.z;
	} else if (abs(normal.y) > EPS) {
		R.x = (double)rand() / (double)RAND_MAX * 1000.0f;
		R.z = (double)rand() / (double)RAND_MAX * 1000.0f;
		R.y = -(normal.x * R.x + normal.z * R.z) / normal.y;
	} else {
		R.y = (double)rand() / (double)RAND_MAX * 1000.0f;
		R.z = (double)rand() / (double)RAND_MAX * 1000.0f;
		R.x = -(normal.y * R.y + normal.z * R.z) / normal.x;
	}
	for (int i=0; i < points->size(); i++) {
		Vector ta = points->at(i) - points->at(((i+1) == points->size())?0:(i+1)),
			   tc = p - points->at(((i+1) == points->size())?0:(i+1));
		if (abs(-ta.x * R.y + ta.y * R.x) > EPS)  {
			x = (- tc.x * R.y + tc.y * R.x) / (-ta.x * R.y + ta.y * R.x);
			y = (ta.x * tc.y - ta.y * tc.x) / (-ta.x * R.y + ta.y * R.x);
		} else if (abs(-ta.x * R.z + ta.z * R.x) > EPS) {
			x = (- tc.x * R.z + tc.z * R.x) / (-ta.x * R.z + ta.z * R.x);
			y = (ta.x * tc.z - ta.z * tc.x) / (-ta.x * R.z + ta.z * R.x);
		} else if (abs(-ta.z * R.y + ta.y * R.z) > EPS) {
			x = (- tc.z * R.y + tc.y * R.z) / (-ta.z * R.y + ta.y * R.z);
			y = (ta.z * tc.y - ta.y * tc.z) / (-ta.z * R.y + ta.y * R.z);
		} else 
			//cerr<<"Bad random ray when mesh intercepting"<<endl;
			;
		if ((x > EPS) & (x < 1.0f)  & (y > 0.0f)) cnt++;
	}
	return (cnt & 1);
}


bool Mesh::getNormal(const Vector &p, Vector &N) {
	// guarantee p is on the mesh
	if (abs(dot(this->N, p - position))>EPS) return false;
	N = this->N;
	return true;
}