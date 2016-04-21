#include <string>
#include <iostream>
#include "RayTracer.h"

using namespace std;

RayTracer::RayTracer() {
	config = new Config();
	image = new Image();
}

RayTracer::RayTracer(string configIn, string imageOut) {
	//setting files
	this->configIn = configIn;
	this->imageOut = imageOut;

	//setting configuration reader
	config = new Config();
	ConfigReader *configReader = new ConfigReader(configIn, config);
	configReader->readConfig();
	delete configReader;

	//setting image
	image = new Image(imageOut, config);

}

RayTracer::~RayTracer() {
	delete config;
	delete image;
}

void RayTracer::run() {
}

void RayTracer::save() {
	image->save();
}