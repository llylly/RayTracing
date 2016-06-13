#ifndef SETFACTORY_H
#define SETFACTORY_H

#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Object.h"
#include "ObjectFactory.h"
#include "Set.h"

class SetFactory {
public:
	SetFactory();
	~SetFactory();

	Set *newSet(pair<map<string, string>, vector<map<string, string>>>);

private:
	void fromFile(Set* (&now));
	Vector pointTrans(const Vector&, Set*, int isNormal = 0);
};

#endif

