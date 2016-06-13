#ifndef OBJECTFACTORY_H
#define OBJECTFACTORY_H

#include <vector>
#include <map>
#include <string>
#include <sstream>
#include "Vector.h"
#include "Color.h"
#include "Object.h"
#include "Sphere.h"
#include "Plane.h"
#include "Mesh.h"
#include "TriMesh.h"
#include "BMP.h"

using namespace std;

class ObjectFactory {
public:
	ObjectFactory();
	~ObjectFactory();

	Object *newObj(const map<string, string>&);
};

#endif
