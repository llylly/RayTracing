#define DEBUG

#include <cstdio>
#include <ctime>
#include <string>
#include <omp.h>
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
	time_t Stime, Ttime;
	Stime = time(0);

	RayTracer* mainTracer = new RayTracer(configIn, imageOut);
	RenderView* renderView = new RenderView(argc, argv, mainTracer->getRenderWidth(), mainTracer->getRenderHeight());
	RenderView::setProxy(mainTracer);
	if (mainTracer->getDisplayOn())
		RenderView::beginLoop();
	else 
		RenderView::display();

	Ttime = time(0);
	cout<<"Time: "<<Ttime - Stime<<" s"<<endl;
	return 0;
}