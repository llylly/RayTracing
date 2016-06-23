#include "RenderView.h"

RayTracer *RenderView::R = 0;
int RenderView::width = 0;
int RenderView::height = 0;

RenderView::RenderView() {
}

RenderView::~RenderView() {
}

RenderView::RenderView(int argc, char** argv, int _width, int _height) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_SINGLE);
	glutInitWindowSize(width = _width, height = _height);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("RayTracing");
	glutDisplayFunc(RenderView::display);
	RenderView::initConfig(width, height);
}

void RenderView::display() {
	glMatrixMode(GL_MODELVIEW);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	static bool rendered = false;
	if ((R) && (!rendered)) {
		rendered = true;
		PhotonMapper::setConfig(RayTracer::getConfig());
		PhotonMapper::setImage(RayTracer::getImage());
		PhotonMapper::setLights(RayTracer::getLights());
		PhotonMapper::setCamera(RayTracer::getCamera());
		PhotonMapper::setObjects(RayTracer::getObjects());
		PhotonMapper::setSets(RayTracer::getSets());
		CausticMapper::setR(RayTracer::getConfig());
		cerr << "PhotonMapper start" << endl;
		PhotonMapper::run();
		cerr << "PhotonMapper end" << endl;
		cerr << "CausticMapper start" << endl;
		CausticMapper::run();
		cerr << "CausticMapper end" << endl;
		cerr << "RayTracer start" << endl;
		R->run();
		cerr << "RayTracer end" << endl;
		cerr << "Saving..." << endl;
		R->save();
		cerr<<"Finish~"<<endl;
	}	
}

void RenderView::beginLoop() {
	glutMainLoop();
}

void RenderView::setProxy(RayTracer *_r) {
	RenderView::R = _r;
}

void RenderView::initConfig(int _w, int _h) {
	glClearColor(0.4784f, 0.4039f, 0.9333f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0f, _w, 0.0f, _h);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glViewport(0, 0, _w, _h);
}

void RenderView::imgDisplay(Image *img, int x, int y) {
	Color tc = img->get(x, y);
	glColor3f(tc.R, tc.G, tc.B);
	glBegin(GL_POINTS);
		glVertex2f(float(x), float(y));
	glEnd();
	glFlush();
}