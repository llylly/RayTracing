#ifndef CONFIGREADER_H
#define CONFIGREADER_H

#include <string>
#include "Config.h"

using namespace std;

class ConfigReader {
public:
	string filePath;

	ConfigReader();
	ConfigReader(string, Config*);
	Config* readConfig();

private:
	Config *config;

	void readItem(const string&, const string&);
};

#endif