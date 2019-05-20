#ifndef _D2Q9_H
#define _D2Q9_H

#include <vector>

class D2Q9 {
public:
    
    // Model constants:
	const int Q     = 9;													    //Discrete velocities
	const double dx = 1.0;														//Lattice spacing
	const double dt = 1.0;														//Time step

	//Weights:
	double w0 = 4.0 / 9.0; double ws = 1.0 / 9.0; double wi = 1.0 / 36.0;
	std::vector<double> w = { w0, ws, ws, ws, ws, wi, wi, wi, wi };			    //Weights:

	//Velocities:
	std::vector<int> cx    = { 0, 1, 0, -1, 0, 1, -1, -1, 1 };					//X-discrete velocity
	std::vector<int> cy    = { 0, 0, 1, 0, -1, 1, 1, -1, -1 };					//Y-discrete velocity
	std::vector<double> uw = { 0,0 };										    //Boundary velocity
};

#endif // !_D2Q9_H
