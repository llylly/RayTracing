#include "SetFactory.h"
#include "Tools.h"

#define PI 3.1415926f

SetFactory::SetFactory() {
}


SetFactory::~SetFactory() {
}

Set *SetFactory::newSet(pair<map<string, string>, vector<map<string, string>>> conf) {
	ObjectFactory *objectFactory = new ObjectFactory();
	Set *now = new Set();

	double diffuseFactor = 0.0f, specularFactor = 0.0f;
	int specularPower = 0;
	double reflectFactor = 0.0f, diffuseReflectValue = 0.0f, environmentFactor = 0.0f;
	double refractFactor = 0.0f, refractN = 1.0f, beerConst = 0.0f;
	Color color = Color(0.0f, 0.0f, 0.0f);
	bool textured = false;
	string texturePath = "";
	bool isUnitedStyle = false;
	string filePath;

	bool fromImport = false;

	double scale = 1.0f;
	Vector displace = Vector(0.0f, 0.0f, 0.0f);
	double XRotateAngle = 0.0f, YRotateAngle = 0.0f, ZRotateAngle = 0.0f;

	int causticTot = 0;

	for (map<string, string>::const_iterator i = conf.first.begin(); i != conf.first.end(); i++) {
		istringstream is(i->second);
		
		if (i->first == "Color") {
			int R, G, B;
			is >> R >> G >> B;
			color = Color((double)R/255.0f, (double)G/255.0f, (double)B/255.0f);
		}
		if (i->first == "UseTexture") {
			int tmp;
			is >> tmp;
			if (tmp) textured = true; else textured = false;
		}
		if (i->first == "TexturePath") {
			int ls = i->second.find_first_of("\""), rs = i->second.find_last_of("\"");
			texturePath = i->second.substr(ls+1, rs-ls-1);
		}
		if (i->first == "DiffuseFactor") {
			is >> diffuseFactor;
		}
		if (i->first == "SpecularFactor") {
			is >> specularFactor;
		}
		if (i->first == "SpecularPower") {
			is >> specularPower;
		}
		if (i->first == "ReflectFactor") {
			is >> reflectFactor;
		}
		if (i->first == "DiffuseReflectValue") {
			is >> diffuseReflectValue;
		}
		if (i->first == "EnvironmentFactor") {
			is >> environmentFactor;
		}
		if (i->first == "RefractFactor") {
			is >> refractFactor;
		}
		if (i->first == "RefractN") {
			is >> refractN;
		}
		if (i->first == "BeerConst") {
			is >> beerConst;
		}
		if (i->first == "UnitedStyle") {
			is >> isUnitedStyle;
		}
		if (i->first == "ImportObj") {
			fromImport = true;
			is >> filePath;
			int l = filePath.find_first_of("\""), r = filePath.find_last_of("\"");
			filePath = filePath.substr(l+1, r-l-1);
		}
		if (i->first == "Displacement") {
			is >> displace.x >> displace.y >> displace.z;
		}
		if (i->first == "Scale") {
			is >> scale;
		}
		if (i->first == "XRotateAngle") {
			is >> XRotateAngle;
		}
		if (i->first == "YRotateAngle") {
			is >> YRotateAngle;
		}
		if (i->first == "ZRotateAngle") {
			is >> ZRotateAngle;
		}
		if (i->first == "CausticTot") {
			is >> causticTot;
		}
	}
	
	for (vector<map<string, string>>::const_iterator i = conf.second.begin(); i != conf.second.end(); i++) {
		now->addObj(objectFactory->newObj(*i));
	}

	BMP *texture;
	if (textured)
		texture = new BMP(texturePath);

	now->bgColor = color,
	now->textured = textured,
	now->texture = texture,
	now->diffuseFactor = diffuseFactor,
	now->specularFactor = specularFactor,
	now->specularPower = specularPower,
	now->reflectFactor = reflectFactor,
	now->environmentFactor = environmentFactor,
	now->diffuseReflectValue = diffuseReflectValue,
	now->refractFactor = refractFactor,
	now->refractN = refractN;
	now->beerConst = beerConst;
	
	now->filePath = filePath;

	now->displace = displace;
	now->scale = scale;
	now->XRotateAngle = XRotateAngle / 180.0f * PI;
	now->YRotateAngle = YRotateAngle / 180.0f * PI;
	now->ZRotateAngle = ZRotateAngle / 180.0f * PI;

	now->causticTot = causticTot;
	
	// read obj file
	if (fromImport) {
		fromFile(now);
	}

	// apply coordinate transition
	for (vector<Object*>::iterator i = now->objSet->begin();  i != now->objSet->end(); i++) {
		(*i)->position = pointTrans((*i)->position, now);
		if ((*i)->type != "Sphere") {
			(*i)->textureOrigin = pointTrans((*i)->textureOrigin, now),
			(*i)->textureXVec = pointTrans((*i)->textureXVec, now, 2),
			(*i)->textureYVec = pointTrans((*i)->textureYVec, now, 2);
		}
		if ((*i)->type == "Plane") {
			((Plane*)(*i))->N = pointTrans(((Plane*)(*i))->N, now, 1);
		}
		if (((*i)->type == "Mesh") || ((*i)->type == "TriMesh")) {
			((Mesh*)(*i))->N = pointTrans(((Mesh*)(*i))->N, now, 1),
			((Mesh*)(*i))->normal = pointTrans(((Mesh*)(*i))->normal, now, 1);
			for (vector<Vector>::iterator j = ((Mesh*)(*i))->points->begin(); j != ((Mesh*)(*i))->points->end(); j++) {
				(*j) = pointTrans((*j), now);
			}
		}
	}

	if (isUnitedStyle) {
		for (vector<Object*>::iterator i = now->objSet->begin();  i != now->objSet->end(); i++) {
			(*i)->bgColor = now->bgColor;
			(*i)->textured = now->textured,
			(*i)->texture = now->texture;
		}
	}
	for (vector<Object*>::iterator i = now->objSet->begin();  i != now->objSet->end(); i++) {
		(*i)->diffuseFactor = now->diffuseFactor,
		(*i)->specularFactor = now->specularFactor,
		(*i)->specularPower = now->specularPower,
		(*i)->reflectFactor = now->reflectFactor,
		(*i)->diffuseReflectValue = now->diffuseReflectValue,
		(*i)->environmentFactor = now->environmentFactor,
		(*i)->refractFactor = now->refractFactor,
		(*i)->refractN = now->refractN;
		(*i)->beerConst = now->beerConst;
	}

	return now;
}

void SetFactory::fromFile(Set* (&now)) {
	ifstream fin(now->filePath);

	vector<Vector> *vque = new vector<Vector>;
	vector<Vector> *vtque = new vector<Vector>;
	vector<Vector> *vnque = new vector<Vector>;
	int totv = 0, totvt = 0, totvn = 0;

	map<string, Color> *colorMap = new map<string, Color>();
	map<string, string> *pathMap = new map<string, string>();
	string nowMtl = "";
	BMP *nowBMP;

	int totface = 0;

	while (!fin.eof()) {
		string nowline;
		getline(fin, nowline);
		istringstream sin(nowline);
		string type;

		if (!(sin >> type)) continue;

		if (type == "#") continue;

		if (type == "mtllib") {
			string mtlPath;
			sin >> mtlPath;
			int last = 0;
			for (int i = 0; i<now->filePath.length(); i++)
				if ((now->filePath[i] == '/') || (now->filePath[i] == '\\'))
					last = i;
			mtlPath = now->filePath.substr(0, last + 1) + mtlPath;
			ifstream mtlin(mtlPath);
			string nowMtl;
			while (!mtlin.eof()) {
				string nowline;
				getline(mtlin, nowline);
				istringstream min(nowline);
				string type = "";
				min >> type;
				if (type == "#") continue;
				if (type == "newmtl") {
					min >> nowMtl;
				}
				if (type == "Kd") {
					Color bgColor;
					min >> bgColor.R >> bgColor.G >> bgColor.B;
					(*colorMap)[nowMtl] = bgColor;
				}
				if (type == "map_Kd") {
					int last = 0;
					for (int i = 0; i<mtlPath.length(); i++)
						if ((mtlPath[i] == '/') || (mtlPath[i] == '\\'))
							last = i;
					string picPath;
					min >> picPath;
					picPath = mtlPath.substr(0, last + 1) + picPath;
					(*pathMap)[nowMtl] = picPath;
				}
			}
		}
		
		if (type == "usemtl") {
			sin >> nowMtl;
			if (pathMap->count(nowMtl))
				nowBMP = new BMP((*pathMap)[nowMtl]);
			else
				nowBMP = 0;
		}

		if (type == "v") {
			Vector nowv;
			sin >> nowv.x >> nowv.y >> nowv.z;
			vque->push_back(nowv); 
			++totv;
		}

		if (type == "vt") {
			Vector nowv;
			sin >> nowv.x >> nowv.y;
			vtque->push_back(nowv);
			++totvt;
		}

		if (type == "vn") {
			Vector nowv;
			sin >> nowv.x >> nowv.y >> nowv.z;
			vnque->push_back(nowv);
			++totvn;
		}

		if (type == "f") {
			++totface;

			vector<Vector> nowpoints = vector<Vector>();
			vector<Vector> nowtpoints = vector<Vector>();
			vector<Vector> nownpoints = vector<Vector>();
			string nows;

			bool hasN = false;

			while (sin >> nows) {
				int fir = nows.find("/"), last = nows.find_last_of("/");
				if (fir == -1) {
					istringstream tmpin(nows);
					int t; tmpin >> t;
					--t;
					nowpoints.push_back(vque->at(t));
				}
				if ((fir != -1) && (fir == last)) {
					istringstream tmpin(nows.substr(0, fir));
					int t; tmpin >> t;
					--t;
					nowpoints.push_back(vque->at(t));
					istringstream tmptin(nows.substr(fir+1, nows.length()-fir-1));
					tmptin >> t;
					--t;
					nowtpoints.push_back(vtque->at(t));
				}
				if ((fir != -1) && (last - fir == 1)) {
					hasN = true;
					istringstream tmpin(nows.substr(0, fir));
					int t; tmpin >> t;
					--t;
					nowpoints.push_back(vque->at(t));
					istringstream tmptin(nows.substr(last+1, nows.length()-last-1));
					tmptin >> t;
					--t;
					nownpoints.push_back(vnque->at(t));
				}
				if ((fir != -1) && (last - fir > 1)) {
					hasN = true;
					istringstream tmpin(nows.substr(0, fir));
					int t; tmpin >> t;
					--t;
					nowpoints.push_back(vque->at(t));
					istringstream tmptin(nows.substr(fir+1, last - fir - 1));
					tmptin >> t;
					--t;
					nowtpoints.push_back(vtque->at(t));
					istringstream tmpnin(nows.substr(last+1, nows.length()-last-1));
					tmpnin >> t;
					--t;
					nownpoints.push_back(vnque->at(t));
				}
			}

			Object *nowObj;
			Color bgColor;
			bool textured = false;
			string texturePath = "";
			Vector textureXVec = Vector(), textureYVec = Vector(), textureOrigin = Vector();

			if (colorMap->count(nowMtl) + pathMap->count(nowMtl) == 0) {
				bgColor = Color(1.0f, 1.0f, 1.0f);
			} else if (pathMap->count(nowMtl) == 0) {
				bgColor = (*colorMap)[nowMtl];
			} else if (nowpoints.size() >= 3) {
				textured = true;
				texturePath = (*pathMap)[nowMtl];
				Vector mPoint[3] = {nowtpoints[0], nowtpoints[1], nowtpoints[2]};
				for (int i=0; i<3; i++) {
					mPoint[i].z = 1;
				}
				Vector coef = triEquationSolver(mPoint[0], mPoint[1], mPoint[2], Vector(0.0f, 0.0f, 1.0f));
				textureOrigin = coef.x * nowpoints[0] + coef.y * nowpoints[1] + coef.z * nowpoints[2];
				
				coef = triEquationSolver(mPoint[0], mPoint[1], mPoint[2], Vector(1.0f, 0.0f, 1.0f));
				textureXVec = coef.x * nowpoints[0] + coef.y * nowpoints[1] + coef.z * nowpoints[2] - textureOrigin;

				coef = triEquationSolver(mPoint[0], mPoint[1], mPoint[2], Vector(0.0f, 1.0f, 1.0f));
				textureYVec = coef.x * nowpoints[0] + coef.y * nowpoints[1] + coef.z * nowpoints[2] - textureOrigin;
			}

			if (nowpoints.size() == 3) {
				nowObj = new TriMesh
					(Vector(0.0f, 0.0f, 0.0f), &nowpoints, nowpoints.size(), bgColor, textured, textureOrigin, textureXVec, textureYVec, nowBMP,
					0.0f, 0.0f, 0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1);
				if (hasN)
					((TriMesh*)nowObj)->setSmooth(nownpoints);
				((TriMesh*)nowObj)->no = totface;
			} else {
				nowObj = new Mesh
				(Vector(0.0f, 0.0f, 0.0f), &nowpoints, nowpoints.size(), bgColor, textured, textureOrigin, textureXVec, textureYVec, nowBMP,
				0.0f, 0.0f, 0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1);
				((Mesh*)nowObj)->no = totface; 
			}
			now->addObj(nowObj);
		}
	}

	fin.close();
	delete vque;
	delete vtque;
	delete vnque;
	delete colorMap;
	delete pathMap;
}

// transition order: XRotate -> YRotate -> ZRotate -> translation
Vector SetFactory::pointTrans(const Vector &p, Set *s, int isNormal) {
	Vector q, q1, q2, q3;
	q = ((isNormal == 1)?1.0f:s->scale) * p;

	q1.x = q.x, 
	q1.y = q.y * cos(s->XRotateAngle) + q.z * sin(s->XRotateAngle),
	q1.z = q.z * cos(s->XRotateAngle) - q.y * sin(s->XRotateAngle);

	q2.x = q1.x * cos(s->YRotateAngle) - q1.z * sin(s->YRotateAngle),
	q2.y = q1.y,
	q2.z = q1.z * cos(s->YRotateAngle) + q1.x * sin(s->YRotateAngle);

	q3.x = q2.x * cos(s->ZRotateAngle) + q2.y * sin(s->ZRotateAngle),
	q3.y = q2.y * cos(s->ZRotateAngle) - q2.x * sin(s->ZRotateAngle),
	q3.z = q2.z;

	if (isNormal)
		return q3;
	else
		return (q3 + s->displace);
}