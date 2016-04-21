#ifndef CONFIG_H
#define CONFIG_H

#include <map>
#include <string>

using namespace std;

class Config {
public:
	int resolutionHeight, resolutionWidth;
	bool antiAnalising;
	int antiAnalisingFactor;

	Config();
	~Config();
	void addObject(const map<string, string> &conf);

private:

};

#endif