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
	double diffuseFactor, specularFactor;
	int specularPower;
	double reflectFactor, environmentFactor;

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
			reflectFactor, environmentFactor);
	} else
	if (Type == "Plane") {
		now = new Plane(normal, position, color, diffuseFactor, specularFactor, specularPower, reflectFactor, environmentFactor);
	} else {
		now = new Object(position, color, diffuseFactor, specularFactor, specularPower, reflectFactor, environmentFactor);
	}
	return now;
}