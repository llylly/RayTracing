#include "LightFactory.h"


LightFactory::LightFactory() {
}


LightFactory::~LightFactory() {
}

Light *LightFactory::newLight(const map<string, string> &conf) {
	Light *light = nullptr;
	if (conf.at("Type") == "Point") {
		Vector position;
		Color color;
		int haloFactor;
		for (map<string, string>::const_iterator i = conf.begin(); i != conf.end(); i++) {
			istringstream is(i->second);
			if (i->first == "Position") {
				double _x, _y, _z;
				is >> _x >> _y >> _z;
				position = Vector(_x, _y, _z);
			}
			if (i->first == "Color") {
				int _r, _g, _b;
				is >> _r >> _g >> _b;
				color = Color((double)_r / 255.0f, (double)_g / 255.0f, (double)_b / 255.0f);
			}
			if (i->first == "HaloFactor") {
				is >> haloFactor;
			}
		}
		light = new PointLight(position, color, haloFactor);
	} else if (conf.at("Type") == "Plane") {
		Vector origin, xVec, yVec, normal;
		int xDiv = 1, yDiv = 1;
		Color color = Color(1.0f, 1.0f, 1.0f);
		for (map<string, string>::const_iterator i = conf.begin(); i != conf.end(); i++) {
			istringstream is(i->second);
			if (i->first == "Origin") {
				double _x, _y, _z;
				is >> _x >> _y >> _z;
				origin = Vector(_x, _y, _z);
			}
			if (i->first == "XVector") {
				double _x, _y, _z;
				is >> _x >> _y >> _z;
				xVec = Vector(_x, _y, _z);
			}
			if (i->first == "YVector") {
				double _x, _y, _z;
				is >> _x >> _y >> _z;
				yVec = Vector(_x, _y, _z);
			}
			if (i->first == "Normal") {
				double _x, _y, _z;
				is >> _x >> _y >> _z;
				normal = Vector(_x, _y, _z);
			}
			if (i->first == "XDiv") {
				is >> xDiv;
			}
			if (i->first == "YDiv") {
				is >> yDiv;
			}
			if (i->first == "Color") {
				int _r, _g, _b;
				is >> _r >> _g >> _b;
				color = Color((double)_r / 255.0f, (double)_g / 255.0f, (double)_b / 255.0f);
			}
		}
		light = new PlaneLight(origin, xVec, yVec, normal, xDiv, yDiv, color); 
	} else {
		light = new Light();
	}
	return light;
}
