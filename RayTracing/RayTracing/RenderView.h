#ifndef RENDERVIEW_H
#define RENDERVIEW_H

#include "GL/gl.h"
#include "GL/glu.h"
#include "GL/glut.h"

#include "RayTracer.h"
#include "PhotonMapper.h"
#include "CausticMapper.h"
#include "Image.h"

class RenderView {
public:
	RenderView();
	RenderView(int argc, char** argv, int _width, int _height);
	~RenderView();

	static void setProxy(RayTracer*);
	static void beginLoop();
	static void imgDisplay(Image*, int, int);
	static void display();

private:
	static RayTracer *R;
	static int width;
	static int height;

	static void initConfig(int, int);

};

#endif