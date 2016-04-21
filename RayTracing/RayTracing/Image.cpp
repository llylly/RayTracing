#include "Image.h"

Image::Image() {
	//default confuguration
	width = height = 300;
	antiAnalisingFactor = 3;
	Rwidth = Rheight = 900;
	outFilePath = "output.bmp";
}

Image::Image(string _outFilePath, Config *config) {
	width = config->resolutionWidth;
	height = config->resolutionHeight;
	if (config->antiAnalising)
		antiAnalisingFactor = config->antiAnalisingFactor;
	else
		antiAnalisingFactor = 1;
	Rwidth = width * antiAnalisingFactor;
	Rheight = height * antiAnalisingFactor;
	outFilePath = _outFilePath;
}

void Image::save() {
}
