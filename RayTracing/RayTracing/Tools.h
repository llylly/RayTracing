#ifndef TOOLS_H
#define TOOLS_H

#include <string>
#include <cstring>
#include "Vector.h"

using namespace std;

double powF(double, int);

string intToString(int);

Vector triEquationSolver(const Vector&, const Vector&, const Vector&, const Vector&);

#endif