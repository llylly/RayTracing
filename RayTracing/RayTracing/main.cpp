#include <cstdio>
#include <string>
#include "RayTracer.h"

using namespace std;

string configIn = "config.txt";
string imageOut = "result.bmp";


int main() {
	RayTracer* mainTracer = new RayTracer(configIn, imageOut);
	while(1);
	return 0;
}