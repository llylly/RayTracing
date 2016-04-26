#include "LightFactory.h"


LightFactory::LightFactory() {
}


LightFactory::~LightFactory() {
}

Light *LightFactory::newLight(const map<string, string> &conf) {
	Light *light = nullptr;
	if (conf.at("Type") == "Point") {
		Vector position;
		for (map<string, string>::const_iterator i = conf.begin(); i != conf.end(); i++) {
			istringstream is(i->second);
			if (i->first == "Position") {
				double _x, _y, _z;
				is >> _x >> _y >> _z;
				position = Vector(_x, _y, _z);
			}
		}
		light = new PointLight(position);
	} else {
		light = new Light();
	}
	return light;
}
