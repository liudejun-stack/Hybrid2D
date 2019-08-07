#ifndef LATTICE_H
#define LATTICE_H

#include <vector>

class Lattice {
public:
    
	//Lattice Constructor:
	Lattice(int _nx=100, int _ny=100, double _dx=1.0, double _dt=1.0) : nx(_nx), ny(_ny), dx(_dx), dt(_dt) {};

	// Methods Declaration;
	int MapGrid(int i, int j);
	int MapFunction(int i, int j, int k);
	void setBoundary(bool _Top, bool _Bottom, bool _Left, bool _Right);

    // Model constants:
	int Q = 9;													   	 //Discrete velocities
	double dx;														 //Lattice spacing
	double dt;														 //Time step

	//Grid Parameters:
	int nx;
	int ny;
	std::vector<bool> Boundary;

	//Weights:
	double w0 = 4.0 / 9.0; double ws = 1.0 / 9.0; double wi = 1.0 / 36.0;
	std::vector<double> w = { w0, ws, ws, ws, ws, wi, wi, wi, wi };			    //Weights:

	//Velocities:
	std::vector<int> cx    = { 0, 1, 0, -1, 0, 1, -1, -1, 1 };					//X-discrete velocity
	std::vector<int> cy    = { 0, 0, 1, 0, -1, 1, 1, -1, -1 };					//Y-discrete velocity
	std::vector<double> uw = { 0,0 };										    //Boundary velocity
};

#endif // !_LATTICE_H
