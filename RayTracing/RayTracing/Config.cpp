#include "Config.h"

Config::Config() {
	objSet = new vector<map<string, string>>;
}

Config::~Config() {
	delete objSet;
}

void Config::addObjectConf(const map<string, string> &conf) {
	objSet->push_back(conf);
}

const vector<map<string, string>> &Config::getObjectConf() {
	return *objSet;
}