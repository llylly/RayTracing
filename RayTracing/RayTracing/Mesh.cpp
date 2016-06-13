#define EPS 1e-6
#define INF 1e+20
#include "Mesh.h"
#include "Tools.h"

Mesh::Mesh() {
	pointTot = 0;
	points = new vector<Vector>;
	box = 0;
	type = "Mesh";
}

Mesh::Mesh(Vector _normal, vector<Vector> *_points, int _tot, Color _color, bool _textured, Vector _textureOrigin, Vector _textureXVec, Vector _textureYVec, BMP *_texture, double _diffuseFactor, double _specularFactor, int _specularPower,
		double _reflectFactor, double _diffuseReflectValue, double _environmentFactor, double _refractFactor, double _refractN, double _beerConst, bool _recalNormal) {
	pointTot = _tot;
	points = new vector<Vector>;
	type = "Mesh";

	for (vector<Vector>::const_iterator i = _points->begin(); i != _points->end(); i++) {
		points->push_back(*i);
	}
	type = "Mesh";
	position = points->at(0);
	bgColor = _color, textured = _textured, textureOrigin = _textureOrigin, textureXVec = _textureXVec, textureYVec = _textureYVec, texture = _texture,
		diffuseFactor = _diffuseFactor, specularFactor = _specularFactor, specularPower = _specularPower,
		reflectFactor = _reflectFactor, diffuseReflectValue = _diffuseReflectValue, environmentFactor = _environmentFactor,
				refractFactor = _refractFactor, refractN = _refractN;
	normal = cross(points->at(1) - points->at(0), points->at(2) - points->at(0));
	normal = normalize(normal);
	// real normal vector
	if ((pointTot >= 3) && (_recalNormal)) {
		N = normal;
	} else {
		N = _normal; // fake normal vector
	}
	if (textured) {
		calTextureVec(normal);
		textureXVecLen = getLength(textureXVec),
			textureYVecLen = getLength(textureYVec);
		//textureXVecLen2 = textureXVecLen * textureXVecLen,
		//	textureYVecLen2 = textureYVecLen * textureYVecLen;
		// gaurantee texture origin in the plane
		textureOrigin += dot(position - textureOrigin, normal) * N;
	}
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
	Vector R = getArbitraryPlaneVec(normal, 1000.0f);

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
	if (abs(dot(this->normal, p - position))>EPS) return false;
	N = this->N;
	return true;
}

Color Mesh::getColor(const Vector &p) {
	if (textured) {
		Vector ans = triEquationSolver(textureXVec, textureYVec, normal, p - textureOrigin);
		return texture->getColor(ans.x, ans.y);
		//return texture->getColor(dot(p - textureOrigin, textureXVec) / textureXVecLen2, dot(p - textureOrigin, textureYVec) / textureYVecLen2);
	} else 
		return bgColor;
}

void Mesh::calEnclosure() {
	double xmax = -INF, xmin = INF, ymax = -INF, ymin = INF, zmax = -INF, zmin = INF;
	for (vector<Vector>::iterator i = points->begin(); i != points->end(); i++) {
		if (i->x > xmax) xmax = i->x;
		if (i->x < xmin) xmin = i->x;
		if (i->y > ymax) ymax = i->y;
		if (i->y < ymin) ymin = i->y;
		if (i->z > zmax) zmax = i->z;
		if (i->z < zmin) zmin = i->z;
	}
	box = new Enclosure(xmin, xmax, ymin, ymax, zmin, zmax);
}