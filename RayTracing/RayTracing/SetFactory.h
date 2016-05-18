#ifndef SETFACTORY_H
#define SETFACTORY_H

#include <map>
#include <vector>
#include "Object.h"
#include "ObjectFactory.h"
#include "Set.h"

class SetFactory {
public:
	SetFactory();
	~SetFactory();

	Set *newSet(pair<map<string, string>, vector<map<string, string>>>);
};

#endif
