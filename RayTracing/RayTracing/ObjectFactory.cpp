#include "ObjectFactory.h"

ObjectFactory::ObjectFactory() {
}


ObjectFactory::~ObjectFactory() {
}

Object *ObjectFactory::newObj(const map<string, string> &conf) {
	Object *now = nullptr;
	if (conf.at("Type") == "Sphere") {
		Vector position;
		Color color;
		double radius;
		double absorbFactor, reflectFactor;
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
			if (i->first == "Radius") {
				is >> radius;
			}
			if (i->first == "AbsorbFactor") {
				is >> absorbFactor;
			}
			if (i->first == "ReflectFactor") {
				is >> reflectFactor;
			}
		}
		now = new Sphere(position, color, radius, absorbFactor, reflectFactor);
	} else if (conf.at("Type") == "Plane") {
		Vector normal, point;
		for (map<string, string>::const_iterator i = conf.begin(); i != conf.end(); i++) {
			istringstream is(i->second);
			if (i->first == "Normal") {
				double _x, _y, _z;
				is >> _x >> _y >> _z;
				normal = Vector(_x, _y, _z);
			}
			if (i->first == "Point") {
				double _x, _y, _z;
				is >> _x >> _y >> _z;
				point = Vector(_x, _y, _z);
			}
		}
		now = new Plane(normal, point);
	} else {
		now = new Object();
	}
	return now;
}