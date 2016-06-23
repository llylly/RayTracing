#include "ObjectFactory.h"
#include "Tools.h"

ObjectFactory::ObjectFactory() {
}


ObjectFactory::~ObjectFactory() {
}

Object *ObjectFactory::newObj(const map<string, string> &conf) {
	Object *now = nullptr;

	string Type;
	Vector position;
	Color color = Color(0.0f, 0.0f, 0.0f);
	double diffuseFactor = 0.0f, specularFactor = 0.0f;
	int specularPower = 0;
	double reflectFactor = 0.0f, environmentFactor = 0.0f;
	double refractFactor = 0.0f, refractN = 1.0f, beerConst = 0.0f;
	double diffuseReflectValue = 0.0f;
	bool textured = false;
	Vector textureOrigin = Vector(0.0f, 0.0f, 0.0f), 
		textureXVec = Vector(0.0f, 0.0f, 0.0f), 
		textureYVec = Vector(0.0f, 0.0f, 0.0f);
	string texturePath = "";

	double radius; // pour le sphere

	Vector normal; // pour le plane

	double causticTot = 0; // pour le sphere et le mesh

	Type = conf.at("Type");

	for (map<string, string>::const_iterator i = conf.begin(); i != conf.end(); i++) {
		istringstream is(i->second);

		if (i->first == "Position") {
			double _x, _y, _z;
			is >> _x >> _y >> _z;
			position = Vector(_x, _y, _z);
		}
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
		if (i->first == "TextureOrigin") {
			is >> textureOrigin.x >> textureOrigin.y >> textureOrigin.z;
		}
		if (i->first == "TextureXVec") {
			is >> textureXVec.x >> textureXVec.y >> textureXVec.z;
		}
		if (i->first == "TextureYVec") {
			is >> textureYVec.x >> textureYVec.y >> textureYVec.z;
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

		if (Type == "Sphere") {
			if (i->first == "Radius") {
				is >> radius;
			}
		}

		if ((Type == "Plane") || (Type == "Mesh") || (Type == "TriMesh")) {
			if (i->first == "Normal") {
				double _x, _y, _z;
				is >> _x >> _y >> _z;
				normal = Vector(_x, _y, _z);
				normal = normalize(normal);
			}
		}

		if (Type == "Sphere") {
			if (i->first == "CausticTot") {
				is >> causticTot;
			}
		}
	}
	BMP *texture;
	if (textured) 
		texture = new BMP(texturePath);
	if (Type == "Sphere") {
		now = new Sphere(position, color,
			textured, textureOrigin, textureXVec, textureYVec, texture,
			radius,
			diffuseFactor, specularFactor, specularPower, 
			reflectFactor, diffuseReflectValue, environmentFactor,
			refractFactor, refractN, beerConst,
			causticTot);
	} else
	if (Type == "Plane") {
		now = new Plane(normal, position, color,
			textured, textureOrigin, textureXVec, textureYVec, texture,
			diffuseFactor, specularFactor, specularPower, reflectFactor, diffuseReflectValue, environmentFactor,
			refractFactor, refractN, beerConst);
	} else {
		now = new Object(position, color, 
			textured, textureOrigin, textureXVec, textureYVec, texture, 
			diffuseFactor, specularFactor, specularPower, reflectFactor, diffuseReflectValue, environmentFactor, refractFactor,
			refractN, beerConst);
	}
	if ((Type == "Mesh") || (Type == "TriMesh")) {
		vector<Vector> *points = new vector<Vector>();
		int tot = 0;
		while (conf.count("P" + intToString(tot))) {
			istringstream is(conf.at("P" + intToString(tot)));
			double a, b, c;
			is >> a >> b >> c;
			points->push_back(Vector(a, b, c));
			tot++;
		}
		if (points->size() == 3)
			now = new TriMesh(normal, points, tot, color,
			textured, textureOrigin, textureXVec, textureYVec, texture,
			diffuseFactor, specularFactor, specularPower, reflectFactor, diffuseReflectValue, environmentFactor,
			refractFactor, refractN, beerConst);
		else
			now = new Mesh(normal, points, tot, color,
			textured, textureOrigin, textureXVec, textureYVec, texture,
			diffuseFactor, specularFactor, specularPower, reflectFactor, diffuseReflectValue, environmentFactor,
			refractFactor, refractN, beerConst);
	}

		
	return now;
}