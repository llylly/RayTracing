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
	double limitCoefficient;
	int diffuseReflectionTimes;
	int totThread;

	bool photonMapping;
	double photonLuminosity;
	int photonTot, photonN;
	double photonR;

	bool causticMapping;
	double causticLuminosity;
	int causticN;
	double causticK, causticR;

	Config();
	~Config();
	void addObjectConf(const map<string, string> &conf);
	const vector<map<string, string>> &getObjectConf();
	void setCameraConf(const map<string, string> &conf);
	const map<string, string> &getCameraConf();
	void addLightConf(const map<string, string> &conf);
	const vector<map<string, string>> &getLightConf();
	void addSet(const map<string, string> &conf, const vector<map<string, string>> (*nowSet));
	const vector<pair<map<string, string>, vector<map<string, string>>>> &getSetConf();

private:
	vector<map<string, string>> *objSet, *lightSet;
	map<string, string> cameraConf;
	vector<pair<map<string, string>, vector<map<string, string>>>> *setSet;
};

#endif