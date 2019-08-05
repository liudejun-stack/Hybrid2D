#ifndef LATTICE_H
#define LATTICE_H

#include "Math.h"
#include <vector>


class Lattice {
public:
	//Constructor:
	Lattice(int _ID, double _latticeSpeed, double _tau, Vec2d _dim, Vec2d _index) {
		ID    = _ID;
		latticeSpeed    = _latticeSpeed;
		tau   = _tau;
		dim   = _dim;
		index = _index;
		solidFraction = 0.0;

		//Set neighbor node
		for (int k = 0; k < Q; k++) {
			aux = index + discreteVelocity[k];
			/*
			aux[0] = (int)index[0] + discreteVelocity[k][0];
			aux[1] = (int)index[1] + discreteVelocity[k][1];
			*/
			
			if (aux[0] == -1)	aux[0] = dim[0] - 1;
			if (aux[1] == -1)	aux[1] = dim[1] - 1;

			if (aux[0] == (int)dim[0])	aux[0] = 0;
			if (aux[1] == (int)dim[1])	aux[1] = 0;

			nCell.push_back(aux[0] + dim[0] * aux[1]);
		}
	}

	//Methods:
	double set_eqFun(double _rho, Vec2d _vel, int k);

	//Cell variables:
	Vec2d  aux;
	Vec2d  dim;
	Vec2d  index;
	Vec2d  vel = Vec2d::Zero();
	double solidFraction;
	double Bn  = 0.0;
	double rho = 0.0;
	bool   Boundary = false;
	bool   isSolid = true;
	int    ID;
	double latticeSpeed;
	double tau;

	//D2Q9 Variables:
	const int Q = 9;
	const std::vector<int>    op               = { 0, 3, 4, 1, 2, 7, 8, 5, 6 };
	const std::vector<double> weight           = { 4.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 1.0 / 36.0, 1.0 / 36.0, 1.0 / 36.0, 1.0 / 36.0 };
	const std::vector<Vec2d>  discreteVelocity = { {0,0}, {1,0}, {0,1}, {-1,0}, {0,-1}, {1,1}, {-1,1}, {-1,-1}, {1,-1} };
	
	std::vector<double> f     = { 0,0,0,0,0,0,0,0,0 };
	std::vector<double> fTemp = { 0,0,0,0,0,0,0,0,0 };
	std::vector<double> omega = { 0,0,0,0,0,0,0,0,0 };
	std::vector<double> nCell;
};
#endif // !LATTICE_H
