#ifndef CONFIG_H
#define CONFIG_H

#include <map>
#include <string>
#include <vector>

using namespace std;

class Config {
public:
	int resolutionHeight, resolutionWidth;
	bool antiAliasing;
	int antiAliasingFactor;
	bool displayOn;

	Config();
	~Config();
	void addObjectConf(const map<string, string> &conf);
	const vector<map<string, string>> &getObjectConf();
	void setCameraConf(const map<string, string> &conf);
	const map<string, string> &getCameraConf();
	void addLightConf(const map<string, string> &conf);
	const vector<map<string, string>> &getLightConf();

private:
	vector<map<string, string>> *objSet, *lightSet;
	map<string, string> cameraConf;
};

#endif