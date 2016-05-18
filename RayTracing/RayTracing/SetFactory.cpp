#include "SetFactory.h"


SetFactory::SetFactory() {
}


SetFactory::~SetFactory() {
}

Set *SetFactory::newSet(pair<map<string, string>, vector<map<string, string>>> conf) {
	ObjectFactory *objectFactory = new ObjectFactory();
	Set *now = new Set();

	double diffuseFactor = 0.0f, specularFactor = 0.0f;
	int specularPower = 0;
	double reflectFactor = 0.0f, environmentFactor = 0.0f;
	double refractFactor = 0.0f, refractN = 1.0f, beerConst = 0.0f;
	Color color = Color(0.0f, 0.0f, 0.0f);
	bool isUnitedStyle = false;
	string filePath;

	for (map<string, string>::const_iterator i = conf.first.begin(); i != conf.first.end(); i++) {
		istringstream is(i->second);
		
		if (i->first == "Color") {
			int R, G, B;
			is >> R >> G >> B;
			color = Color((double)R/255.0f, (double)G/255.0f, (double)B/255.0f);
		}
		if (i->first == "DiffuseFactor") {
			is >> diffuseFactor;
		}
		if (i->first == "SpecularFactor") {
			is >> specularFactor;
		}
		if (i->first == "SpecularPower") {
			is >> specularPower;
		}
		if (i->first == "ReflectFactor") {
			is >> reflectFactor;
		}
		if (i->first == "EnvironmentFactor") {
			is >> environmentFactor;
		}
		if (i->first == "RefractFactor") {
			is >> refractFactor;
		}
		if (i->first == "RefractN") {
			is >> refractN;
		}
		if (i->first == "BeerConst") {
			is >> beerConst;
		}
		if (i->first == "UnitedStyle") {
			is >> isUnitedStyle;
		}
		if (i->first == "ImportObj") {
			is >> filePath;
			int l = filePath.find_first_of("\""), r = filePath.find_last_of("\"");
			filePath = filePath.substr(l+1, r-l-1);
		}
	}
	
	for (vector<map<string, string>>::const_iterator i = conf.second.begin(); i != conf.second.end(); i++) {
		now->addObj(objectFactory->newObj(*i));
	}

	now->bgColor = color,
	now->diffuseFactor = diffuseFactor,
	now->specularFactor = specularFactor,
	now->specularPower = specularPower,
	now->reflectFactor = reflectFactor,
	now->environmentFactor = environmentFactor,
	now->refractFactor = refractFactor,
	now->refractN = refractN;
	now->beerConst = beerConst;

	if (isUnitedStyle) {
		for (vector<Object*>::iterator i = now->objSet->begin();  i != now->objSet->end(); i++) {
			(*i)->bgColor = now->bgColor;
			(*i)->diffuseFactor = now->diffuseFactor,
			(*i)->specularFactor = now->specularFactor,
			(*i)->specularPower = now->specularPower,
			(*i)->reflectFactor = now->reflectFactor,
			(*i)->environmentFactor = now->environmentFactor,
			(*i)->refractFactor = now->refractFactor,
			(*i)->refractN = now->refractN;
			(*i)->beerConst = now->beerConst;
		}
	}

	return now;
}