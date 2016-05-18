#include "Set.h"


Set::Set() {
	objSet = new vector<Object*>();
}


Set::~Set() {
	for (vector<Object*>::iterator i = objSet->begin(); i != objSet->end(); i++)
		delete (*i);
	delete objSet;
}

void Set::addObj(Object *obj) {
	objSet->push_back(obj);
}
