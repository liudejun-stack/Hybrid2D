#ifndef LATTICE_H
#define LATTICE_H

#include <vector>
<<<<<<< HEAD
#include <iostream>
=======
>>>>>>> 0a3f611cb59ffa7067adf92b5f8a8d64cd2e8eb8
#include "Math.hpp"

class Lattice {
public:

	//Lattice Constructor:
	Lattice(Vec3i _index, Vec3i _dim, double _cs, double _tau) {
		Boundary = false;
		Index    = _index;
		dim      = _dim;
		cs       = _cs;
		tau      = _tau;

		nQ.reserve(Q);
		//Neighbor node:
		for (int k = 0; k < Q; k++) {
			Vec3i nNode;
			nNode[0] = Index[0] + cx[k];
			nNode[1] = Index[1] + cy[k];
			nNode[2] = 0;
			if (nNode[0] == -1)	nNode[0] = dim[0] - 1;
			if (nNode[0] == dim[0])	nNode[0] = 0;
			if (nNode[1] == -1)	nNode[1] = dim[1] - 1;
			if (nNode[1] == dim[1])	nNode[1] = 0;
			if (nNode[2] == -1)	nNode[2] = dim[2] - 1;
			if (nNode[2] == dim[2])	nNode[2] = 0;

			nQ.push_back(nNode[0] + nNode[1] * dim[0] + nNode[2] * dim[0] * dim[1]);
		}
	}

	// Methods Declaration;
	void   cellInfo();
    double   macroUpdate(Vec3d& _vel);
	void   setInitCond(double _rho, Vec3d& _vel);
	double setEqFun(double _rho, Vec3d& _vel, int k);

	//Parameters:,
	Vec3i  Index;			//index i, j, k 
	Vec3i  dim;				//Grid size (nx, ny, nz)
	Vec3d  vel;				//Velocity vector
	double cs;				//Lattice speed (dx/dt)
	double tau;				//Relaxation time
	double rho;				//Cell density
	bool   Boundary;		//Boundary condition (solid or fluid)	

	//D2Q9 parameters:
	int    Q = 9;
	double w0 = 4.0 / 9.0;
	double ws = 1.0 / 9.0;
	double wi = 1.0 / 36.0;
	std::vector<double> w = { w0, ws, ws, ws, ws, wi, wi, wi, wi };
	std::vector<int>    cx = { 0, 1, 0, -1, 0, 1, -1, -1, 1 };
	std::vector<int>    cy = { 0, 0, 1, 0, -1, 1, 1, -1, -1 };
	std::vector<int>    op = { 0, 3, 4, 1, 2, 7, 8, 5, 6 };
	std::vector<double> f = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	std::vector<double> fTemp = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	std::vector<int>    nQ;
};

#endif // !_LATTICE_H
