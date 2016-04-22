#include <cstdio>
#include <string>
#include "RayTracer.h"

using namespace std;

string configIn = "config.txt";
string imageOut = "result.bmp";

int main(int argc, char **argv) {
	RayTracer* mainTracer = new RayTracer(configIn, imageOut);
	mainTracer->run();
	mainTracer->save();
	return 0;
}