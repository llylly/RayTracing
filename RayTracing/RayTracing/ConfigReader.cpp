#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include "ConfigReader.h"

using namespace std;

ConfigReader::ConfigReader(string path, Config *config) {
	this->filePath = path;
	this->config = config;
}

Config* ConfigReader::readConfig() {

	ifstream fin(filePath);
	bool inObjectBody = false, inCameraBody = false, inLightBody = false, inCommentBody = false, inSetBody = false;
	map<string, string> objConf, cameraConf, lightConf, setConf;
	vector<map<string, string>> *nowSet = new vector<map<string, string>>;

	while (!fin.eof()) {
		string now, leftValue, rightValue;
		getline(fin, now);
		if (now.find("/*") != -1) {
			inCommentBody = true;
		}
		if (now.find("*/") != -1) {
			inCommentBody = false;
		}
		if ((now.find("//") != -1) || (inCommentBody)) {
			continue;
		}
		if ((now.find("Object") != -1) && (now.find("{") != -1)) {
			inObjectBody = true;
			objConf.clear();
			continue;
		}
		if ((now.find("Camera") != -1) && (now.find("{") != -1)) {
			inCameraBody = true;
			cameraConf.clear();
		}
		if ((now.find("Light") != -1) && (now.find("{") != -1)) {
			inLightBody = true;
			lightConf.clear();
		}
		if ((now.find("Set") != -1) && (now.find("{") != -1)) {
			inSetBody = true;
			setConf.clear();
			nowSet->clear();
		}
		if (now.find("}") != -1) {
			if (inObjectBody) {
				inObjectBody = false;
				if (inSetBody)
					nowSet->push_back(objConf);
				else
					config->addObjectConf(objConf);
			} else
			if (inSetBody) {
				inSetBody = false;
				config->addSet(setConf, nowSet);
			}
			if (inCameraBody) 
				inCameraBody = false;
			if (inLightBody) {
				inLightBody = false;
				config->addLightConf(lightConf);
			}
			continue;
		}
		if (now.find("=") != -1) {
			leftValue = "";
			for (int i=0; i<now.find("="); i++) {
				if ((i) && (now[i] != ' ') && (now[i-1] == ' '))
					leftValue = "";
				if (now[i] > 32) 
					leftValue.append(1, now[i]);
			}
			int l = now.find("=") + 1, r= now.length() - 1;
			while ((now[l] <= 32) && (l < now.length())) ++l;
			while ((now[r] <= 32) && (l <= r)) --r;
			if (l <= r)
				rightValue = now.substr(l, r-l+1);
			else 
				rightValue = "";
			
			if (inObjectBody)
				objConf[leftValue] = rightValue;
			else if (inCameraBody)
				cameraConf[leftValue] = rightValue;
			else if (inLightBody)
				lightConf[leftValue] = rightValue;
			else if (inSetBody)
				setConf[leftValue] = rightValue;
			else
				readItem(leftValue, rightValue);

		}
	}
	fin.close();
	config->setCameraConf(cameraConf);
	delete nowSet;
	return this->config;
}

void ConfigReader::readItem(const string& name, const string &value) {
	if (name == "Resolution") {
		int epos = value.find('x'), l, r;
		string sWidth = "", sHeight = "";
		
		l = 0, r = epos - 1;
		while ((value[l] <= 32) && (l<=r)) ++l;
		while ((value[r] <= 32) && (l<=r)) --r;
		if (l<=r) sWidth = value.substr(l, r-l+1);

		l = epos + 1, r = value.length() - 1;
		while ((value[l] <= 32) && (l<=r)) ++l;
		while ((value[r] <= 32) && (l<=r)) --r;
		if (l<=r) sHeight= value.substr(l, r-l+1);

		if (sWidth != "")
			config->resolutionWidth = stoi(sWidth);
		else
			config->resolutionWidth = 300; //default configuration
		
		if (sHeight != "")
			config->resolutionHeight = stoi(sHeight);
		else
			config->resolutionHeight = 300; //default configuration
	}
	if (name == "AntiAliasingOn") {
		char able = '0';
		for (int i=0; i<value.length(); i++)
			if ((value[i] >= '0') && (value[i] <= '9'))
				able = value[i];
		if (able == '0') 
			config->antiAliasing = false;
		else
			config->antiAliasing = true;
	}
	if (name == "AntiAliasingFactor") {
		int num = 0;
		for (int i=0; i<value.length(); i++)
			if ((value[i] >= '0') && (value[i] <= '9'))
				num = num * 10 + value[i] - '0';
		config->antiAliasingFactor = num;
	}
	if (name == "DisplayOn") {
		char able = '0';
		for (int i=0; i<value.length(); i++)
			if ((value[i] >= '0') && (value[i] <= '9'))
				able = value[i];
		if (able == '0') 
			config->displayOn = false;
		else
			config->displayOn = true;
	}
	if (name == "DiffuseReflectTimes") {
		istringstream is(value);
		is >> config->diffuseReflectionTimes;
	}
	if (name == "LimitCoefficient") {
		istringstream is(value);
		is >> config->limitCoefficient;
	}
	if (name == "TotThread") {
		istringstream is(value);
		is >> config->totThread;
	}
	if (name == "PhotonMapping") {
		istringstream is(value);
		is >> config->photonMapping;
	}
	if (name == "PhotonLuminosity") {
		istringstream is(value);
		is >> config->photonLuminosity;
	}
	if (name == "PhotonTot") {
		istringstream is(value);
		is >> config->photonTot;
	}
	if (name == "PhotonR") {
		istringstream is(value);
		is >> config->photonR;
	}
	if (name == "PhotonN") {
		istringstream is(value);
		is >> config->photonN;
	}
	if (name == "CausticMapping") {
		istringstream is(value);
		is >> config->causticMapping;
	}
	if (name == "CausticLuminosity") {
		istringstream is(value);
		is >> config->causticLuminosity;
	}
	if (name == "CausticK") {
		istringstream is(value);
		is >> config->causticK;
	}
	if (name == "CausticR") {
		istringstream is(value);
		is >> config->causticR;
	}
	if (name == "CausticN") {
		istringstream is(value);
		is >> config->causticN;
	}
}