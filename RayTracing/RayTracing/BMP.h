#ifndef BMP_H
#define BMP_H

#include <string>
#include <fstream>
#include <iostream>
#include "Color.h"

using namespace std;

class BMP {
public:
	BMP();
	BMP(string);
	~BMP();
	void open(string);
	const Color &get(int, int);
	int Width();
	int Height();
	
private:
	string filePath;
	int width, height;
	Color **arr;

	inline void spaceClear();
};

#endif