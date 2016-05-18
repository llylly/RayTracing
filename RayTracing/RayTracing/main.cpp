#define DEBUG

#include <cstdio>
#include <ctime>
#include <string>
#include "RayTracer.h"

#include "RenderView.h"

using namespace std;

string configIn = "config.txt";
string imageOut = "result.bmp";

int main(int argc, char **argv) {
#ifdef DEBUG
	freopen("msg.txt", "w", stdout);
#endif
	srand(time(0));

	RayTracer* mainTracer = new RayTracer(configIn, imageOut);
	
	RenderView* renderView = new RenderView(argc, argv, mainTracer->getRenderWidth(), mainTracer->getRenderHeight());
	RenderView::setProxy(mainTracer);
	if (mainTracer->getDisplayOn())
		RenderView::beginLoop();
	else 
		RenderView::display();
	return 0;
}