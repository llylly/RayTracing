#define DEBUG

#include <string>
#include <iostream>
#include "RayTracer.h"
#include "RenderView.h"

using namespace std;

RayTracer::RayTracer() {
	config = new Config();
	image = new Image();
	objects.clear();
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
	image->spaceInit();

	//setting camera
	camera = new Camera(config->getCameraConf(), getRenderWidth(), getRenderHeight());
#ifdef DEBUG
	cout<<"------CAMERA------"<<endl;
	cout<<"Eye: "<<camera->eye<<endl;
	cout<<"Center: "<<camera->center<<endl;
	cout<<"XVector: "<<camera->xDirec<<endl;
	cout<<"YVector: "<<camera->yDirec<<endl;
	cout<<"------------------"<<endl<<endl;
#endif

	//setting objects
	ObjectFactory *objectFactory = new ObjectFactory();
	for (vector<map<string, string>>::const_iterator i = config->getObjectConf().begin(); i != config->getObjectConf().end(); i++)
		objects.push_back(objectFactory->newObj(*i));

	//setting lights
	LightFactory *lightFactory = new LightFactory();
	for (vector<map<string, string>>::const_iterator i = config->getLightConf().begin(); i != config->getLightConf().end(); i++)
		lights.push_back(lightFactory->newLight(*i));
}

RayTracer::~RayTracer() {
	delete config;
	delete image;
	delete camera;
}

void RayTracer::save() {
	image->antiAliasing();
	image->save();
}

int RayTracer::getRenderWidth() {
	return config->resolutionWidth * config->antiAliasingFactor;
}

int RayTracer::getRenderHeight() {
	return config->resolutionHeight * config->antiAliasingFactor;
}

bool RayTracer::getDisplayOn() {
	return config->displayOn;
}

void RayTracer::run() {
	for (int i=0; i<400; i++)
		for (int j=0; j<300; j++) {
			image->set(Color((float)(rand())/(float)RAND_MAX, (float)(rand())/(float)RAND_MAX, (float)(rand())/(float)RAND_MAX), i, j);
			if (config->displayOn)
				RenderView::imgDisplay(image, i, j);
		}
}
