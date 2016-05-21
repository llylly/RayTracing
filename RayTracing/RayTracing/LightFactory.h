#ifndef LIGHTFACTORY_H
#define LIGHTFACTORY_H

#include <map>
#include <string>
#include <sstream>
#include "Vector.h"
#include "Light.h"
#include "PointLight.h"
#include "PlaneLight.h"
#include "Color.h"

using namespace std;

class LightFactory {
public:
	LightFactory();
	~LightFactory();

	Light *newLight(const map<string, string>&);
};

#endif
