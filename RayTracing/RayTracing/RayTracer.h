#ifndef RAY_TRACER_H
#define RAY_TRACER_H

#include <string>
#include "ConfigReader.h"
#include "Config.h"
#include "Image.h"

using namespace std;

struct RayTracer {
public:
	string configIn, imageOut;

	RayTracer();
	RayTracer(string configIn, string imageOut);
	~RayTracer();
	void run();
	void save();

private:
	Config *config;
	Image *image;
};
#endif