#include "BMP.h"


BMP::BMP() {
	width = height = 0;
	arr = 0;
}

BMP::BMP(string _path) {
	arr = 0;
	open(_path);
}

void BMP::open(string _path) {
	filePath = _path;
	ifstream fin(filePath, ios::in | ios::binary);
	char nowc;
	for (int i=0; i<18; i++)
		nowc = fin.get();
	int x = 0, pow = 1;
	for (int i=0; i<4; i++) {
		nowc = fin.get();
		if (nowc<0) x += pow * (int(nowc) + 256); else x += pow*nowc;
		pow*=256;
	}
	width = x;

	x = 0, pow = 1;
	for (int i=0; i<4; i++) {
		nowc = fin.get();
		if (nowc<0) x += pow * (int(nowc) + 256); else x += pow*nowc;
		pow*=256;
	}
	height = x;
	
	bool toned = false;
	if (height<0) height = -height, toned = true; 

	for (int i=0; i<2; i++)
		nowc = fin.get();
	fin.get(nowc);

	int digit = nowc;

	for (int i=0; i<25; i++)
		nowc = fin.get();

	if (arr) spaceClear();
	arr = new Color*[height];
	for (int i=0; i<height; i++)
		arr[i] = new Color[width];

	int i, j;
	if (toned) i = height-1, j = width-1; else i=j=0;
	while (1) {
		int lineTot = width * ((digit==24)?3:4);
		while (1) {
			nowc = fin.get();
			arr[i][j].R = (double)(nowc<0?256+nowc:nowc) / 255.0;
			nowc = fin.get();
			arr[i][j].G = (double)(nowc<0?256+nowc:nowc) / 255.0;
			nowc = fin.get();
			arr[i][j].B = (double)(nowc<0?256+nowc:nowc) / 255.0;
			if (digit!=24)
				nowc = fin.get();
			if (toned) --j; else ++j;
			if ((toned) && (j<0)) break;
			if ((!toned) && (j>=width)) break;
		}
		if (lineTot & 3)
			for (int j = 0; j<(4 - (lineTot&3)); j++)
				nowc = fin.get();
		if (toned) --i, j = width-1; else ++i, j = 0;
		if ((toned) && (i<0)) break;
		if ((!toned) && (i>=height)) break;
	}

	fin.close();
}


BMP::~BMP() {
	spaceClear();
}

inline void BMP::spaceClear() {
	for (int i=0; i<height; i++)
		delete[] arr[i];
	delete[] arr;
}

const Color &BMP::get(int x, int y) {
	return arr[x][y];
}

int BMP::Width() {
	return width;
}

int BMP::Height() {
	return height;
}