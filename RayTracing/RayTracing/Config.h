#ifndef CONFIG_H
#define CONFIG_H

#include <map>
#include <string>
#include <vector>

using namespace std;

class Config {
public:
	int resolutionHeight, resolutionWidth;
	bool antiAnalising;
	int antiAnalisingFactor;

	Config();
	~Config();
	void addObjectConf(const map<string, string> &conf);
	const vector<map<string, string>> &getObjectConf();

private:
	vector<map<string, string>> *objSet;
};

#endif