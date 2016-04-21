#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include "Config.h"

using namespace std;

class Image {
public:
	Image();
	Image(string, Config*);
	void save();

private:
	int width, height, Rwidth, Rheight;
	int antiAnalisingFactor;
	string outFilePath;
};

#endif

