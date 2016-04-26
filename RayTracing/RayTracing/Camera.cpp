#include "Camera.h"


Camera::Camera() {
}

Camera::Camera(const map<string, string> &conf, int viewWidth, int viewHeight) {
	double xLength, yLength;
	Vector orient;

	for (map<string, string>::const_iterator i = conf.begin(); i != conf.end(); i++) {
		istringstream is(i->second);
		if (i->first == "EyePosition") {
			double _x, _y, _z;
			is >> _x >> _y >> _z;
			eye = Vector(_x, _y, _z);
		}
		if (i->first == "Position") {
			double _x, _y, _z;
			is >> _x >> _y >> _z;
			center = Vector(_x, _y, _z);
		}
		if (i->first == "XLength") {
			is >> xLength;
		}
		if (i->first == "Orient") {
			double _x, _y, _z;
			is >> _x >> _y >> _z;
			orient = Vector(_x, _y, _z);
		}
	}

	yLength = xLength / (double)viewWidth * (double)viewHeight;
	xDirec = cross(center - eye, orient);
	xDirec = normalize(xDirec, xLength / 2.0f);
	yDirec = cross(xDirec, center - eye);
	yDirec = normalize(yDirec, yLength / 2.0f);
}

Camera::~Camera() {
}
