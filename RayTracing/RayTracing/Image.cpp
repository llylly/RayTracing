#include "Image.h"

Image::Image() {
	//default confuguration
	width = height = 300;
	antiAliasingFactor = 3;
	Rwidth = Rheight = 900;
	outFilePath = "output.bmp";
	arr = 0;
}

Image::Image(string _outFilePath, Config *config) {
	width = config->resolutionWidth;
	height = config->resolutionHeight;
	if (config->antiAliasing)
		antiAliasingFactor = config->antiAliasingFactor;
	else
		antiAliasingFactor = 1;
	Rwidth = width * antiAliasingFactor;
	Rheight = height * antiAliasingFactor;
	outFilePath = _outFilePath;
	arr = 0;
}

Image::~Image() {
	spaceClear();
}

void Image::spaceInit() {
	if (arr != 0) spaceClear();
	arr = new Color*[Rheight];
	for (int i=0; i<Rheight; i++)
		arr[i] = new Color[Rwidth];
}

void Image::spaceClear() {
	for (int i=0; i<Rheight; i++)
		delete[] arr[i];
	delete[] arr;
}

void Image::save() {
	ofstream fout(outFilePath, ios::binary | ios::out);

	char emptyC[20];
	memset(emptyC, 0, sizeof(emptyC));

	fout.write("BM", 2);
	int sizePos = fout.tellp(), totSize = 54;
	fout.write(emptyC, 8);
	fout.put(54);
	fout.write(emptyC, 3);

	fout.put(40);
	fout.write(emptyC, 3);
	fout.put((char)(width%256));
	fout.put((char)(width/256%256));
	fout.put((char)(width/65536%256));
	fout.put((char)(width/16777216));
	fout.put((char)(height%256));
	fout.put((char)(height/256%256));
	fout.put((char)(height/65536%256));
	fout.put((char)(height/16777216%256));
	fout.put(1), fout.put(0);
	fout.put(24), fout.put(0);
	fout.write(emptyC, 4);
	fout.write(emptyC, 4);
	fout.write(emptyC, 16);

	for (int i=0; i<height; i++) {
		int lineData = 3 * width;
		totSize += lineData;
		for (int j=0; j<width; j++) {
			int tr = arr[i][j].B * 256.0 - 0.50;
			tr = (tr<0)?0:tr;
			fout.put((char)tr);
			tr = arr[i][j].G * 256.0 - 0.50;
			tr = (tr<0)?0:tr;
			fout.put((char)tr);
			tr = arr[i][j].R * 256.0 - 0.50;
			tr = (tr<0)?0:tr;
			fout.put((char)tr);
		}
		if (lineData & 3) {
			totSize += 4 - lineData & 3;
			fout.write(emptyC, 4 - lineData & 3);
		}
	}

	fout.seekp(sizePos, ios::beg);
	while (totSize) {
		fout.put((char)(totSize % 256));
		totSize /= 256;
	}

	fout.close();
}

void Image::set(const Color &c, int x, int y) {
	arr[y][x] = c;
}

int Image::Width() {
	return Rwidth;
}

int Image::Height() {
	return Rheight;
}

void Image::antiAliasing() {
	if (antiAliasingFactor == 1) return;
	double **vmap = new double*[antiAliasingFactor];
	double mapSum = 0.0f;
	for (int i=0; i<antiAliasingFactor; i++)
		vmap[i] = new double[antiAliasingFactor];
	
	double x = 0.5f, y = 0.5f;
	for (int i=0; i<antiAliasingFactor; i++) {
		if (i*2<antiAliasingFactor) x*=2.0f;
		if (i*2>antiAliasingFactor) x/=2.0f;
		y = 0.5f;
		for (int j=0; j<antiAliasingFactor; j++) {
			if (j*2<antiAliasingFactor) y*=2.0f;
			if (j*2>antiAliasingFactor) y/=2.0f;
			vmap[i][j] = x*y, mapSum += vmap[i][j];
		}
	}
	for (int i=0; i<antiAliasingFactor; i++)
		for (int j=0; j<antiAliasingFactor; j++)
			vmap[i][j]/=mapSum;

	Color **newArr = new Color*[height];
	Color tmp;
	for (int i=0; i<height; i++) 
		newArr[i] = new Color[width];
	for (int i=0; i<height; i++)
		for (int j=0; j<width; j++) {
			tmp.set(0.0, 0.0, 0.0);
			for (int ii=0; ii<antiAliasingFactor; ii++)
				for (int jj=0; jj<antiAliasingFactor; jj++)
					tmp+=(arr[i*antiAliasingFactor + ii][j*antiAliasingFactor + jj] * vmap[ii][jj]);
			newArr[i][j] = tmp;
		}

	for (int i=0; i<Rheight; i++)
		delete[] arr[i];
	delete[] arr;
	arr = new Color*[height];
	for (int i=0; i<height; i++)
		arr[i] = newArr[i];
	delete[] newArr;
	for (int i=0; i<antiAliasingFactor; i++)
		delete[] vmap[i];
	delete[] vmap;
}

Color Image::get(int x, int y) {
	return arr[y][x];
}