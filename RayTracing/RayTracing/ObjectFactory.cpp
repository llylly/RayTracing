#include "ObjectFactory.h"

ObjectFactory::ObjectFactory() {
}


ObjectFactory::~ObjectFactory() {
}

Object *ObjectFactory::newObj(const map<string, string> &conf) {
	Object *now = nullptr;

	string Type;
	Vector position;
	Color color;
	double diffuseFactor = 0.0f, specularFactor = 0.0f;
	int specularPower = 0.0f;
	double reflectFactor = 0.0f, environmentFactor = 0.0f;
	double refractFactor = 0.0f, refractN = 1.0f;

	double radius; // pour le sphere

	Vector normal; // pour le plane

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
		if (i->first == "EnvironmentFactor") {
			is >> environmentFactor;
		}
		if (i->first == "RefractFactor") {
			is >> refractFactor;
		}
		if (i->first == "RefractN") {
			is >> refractN;
		}

		if (conf.at("Type") == "Sphere") {
			if (i->first == "Radius") {
				is >> radius;
			}
		}

		if (conf.at("Type") == "Plane") {
			if (i->first == "Normal") {
				double _x, _y, _z;
				is >> _x >> _y >> _z;
				normal = Vector(_x, _y, _z);
				normal = normalize(normal);
			}
		}
	};
	if (Type == "Sphere") {
		now = new Sphere(position, color, radius, diffuseFactor, specularFactor, specularPower, 
			reflectFactor, environmentFactor, refractFactor, refractN);
	} else
	if (Type == "Plane") {
		now = new Plane(normal, position, color, diffuseFactor, specularFactor, specularPower, reflectFactor, environmentFactor, refractFactor, refractN);
	} else {
		now = new Object(position, color, diffuseFactor, specularFactor, specularPower, reflectFactor, environmentFactor, refractFactor, refractN);
	}
	return now;
}