#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include "ConfigReader.h"

using namespace std;

ConfigReader::ConfigReader(string path, Config *config) {
	this->filePath = path;
	this->config = config;
}

Config* ConfigReader::readConfig() {

	ifstream fin(filePath);
	bool inObjectBody = false;
	map<string, string> objConf;

	while (!fin.eof()) {
		string now, leftValue, rightValue;
		getline(fin, now);
		if (now.find("//") != -1) {
			continue;
		}
		if ((now.find("object") != -1) && (now.find("{") != -1)) {
			inObjectBody = true;
			objConf.clear();
			continue;
		}
		if (now.find("}") != -1) {
			inObjectBody = false;
			config->addObjectConf(objConf);
			continue;
		}
		if (now.find("=") != -1) {
			leftValue = "";
			for (int i=0; i<now.find("="); i++) {
				if ((i) && (now[i] != ' ') && (now[i-1] == ' '))
					leftValue = "";
				if (now[i]!=' ') 
					leftValue.append(1, now[i]);
			}
			int l = now.find("=") + 1, r= now.length() - 1;
			while ((now[l] == ' ') && (l < now.length())) ++l;
			while ((now[r] == ' ') && (l <= r)) --r;
			if (l <= r)
				rightValue = now.substr(l, r-l+1);
			else 
				rightValue = "";
			
			if (inObjectBody)
				objConf[leftValue] = rightValue;
			else
				readItem(leftValue, rightValue);

		}
	}
	fin.close();

	return this->config;
}

void ConfigReader::readItem(const string& name, const string &value) {
	if (name == "Resolution") {
		int epos = value.find('x'), l, r;
		string sWidth = "", sHeight = "";
		
		l = 0, r = epos - 1;
		while ((value[l] == ' ') && (l<=r)) ++l;
		while ((value[r] == ' ') && (l<=r)) --r;
		if (l<=r) sWidth = value.substr(l, r-l+1);

		l = epos + 1, r = value.length() - 1;
		while ((value[l] == ' ') && (l<=r)) ++l;
		while ((value[r] == ' ') && (l<=r)) --r;
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
	if (name == "AntiAnalisingOn") {
		char able = '0';
		for (int i=0; i<value.length(); i++)
			if ((value[i] >= '0') && (value[i] <= '9'))
				able = value[i];
		if (able == '0') 
			config->antiAnalising = false;
		else
			config->antiAnalising = true;
	}
	if (name == "AntiAnalisingFactor") {
		int num = 0;
		for (int i=0; i<value.length(); i++)
			if ((value[i] >= '0') && (value[i] <= '9'))
				num = num * 10 + value[i] - '0';
		config->antiAnalisingFactor = num;
	}
	if (name == "CameraPos") {
	}
}