#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include <fstream>
#include <iostream>
#include "Config.h"
#include "Color.h"

using namespace std;

class Image {
public:
	Image();
	Image(string, Config*);
	~Image();
	void save();
	void spaceInit();
	void antiAliasing();
	void set(const Color&, int, int);
	Color get(int x, int y);
	int Width();
	int Height();

private:
	int width, height, Rwidth, Rheight;
	int antiAliasingFactor;
	string outFilePath;
	Color **arr;

	void spaceClear();
};

#endif

