#include "Config.h"

Config::Config() {
	objSet = new vector<map<string, string>>;
	lightSet = new vector<map<string, string>>;
	//set limit coefficient(to be read in config file)
	limitCoefficient = 0.001f;
}

Config::~Config() {
	delete objSet;
	delete lightSet;
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