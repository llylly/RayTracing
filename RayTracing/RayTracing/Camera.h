#ifndef CAMERA_H
#define CAMERA_H

#include <map>
#include <string>
#include <sstream>
#include "Vector.h"

using namespace std;

class Camera {
public:
	Vector eye, center, xDirec, yDirec;

	Camera();
	Camera(const map<string, string> &conf, int viewWidth, int viewHeight);
	~Camera();


};

#endif