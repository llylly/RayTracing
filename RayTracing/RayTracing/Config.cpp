#include "Config.h"

Config::Config() {
	objSet = new vector<map<string, string>>;
	lightSet = new vector<map<string, string>>;
	setSet = new vector<pair<map<string, string>, vector<map<string, string>>>>;
}

Config::~Config() {
	delete objSet;
	delete lightSet;
	delete setSet;
}

void Config::addObjectConf(const map<string, string> &conf) {
	objSet->push_back(conf);
}

const vector<map<string, string>> &Config::getObjectConf() {
	return *objSet;
}

void Config::setCameraConf(const map<string, string> &conf) {
	cameraConf = conf;
}

const map<string, string> &Config::getCameraConf() {
	return cameraConf;
}

void Config::addLightConf(const map<string, string> &conf) {
	lightSet->push_back(conf);
}

const vector<map<string, string>> &Config::getLightConf() {
	return *lightSet;
}

void Config::addSet(const map<string, string> &conf, const vector<map<string, string>> (*nowSet)) {
	setSet->push_back(make_pair(conf, *nowSet));
}

const vector<pair<map<string, string>, vector<map<string, string>>>> &Config::getSetConf() {
	return *setSet;
}

