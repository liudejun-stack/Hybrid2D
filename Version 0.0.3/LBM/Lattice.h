#ifndef LATTICE_H
#define LATTICE_H

#include "Math.h"
#include <vector>


class Lattice {
public:
	//Constructor:
	Lattice(int _ID, double _cs, double _tau, Vec2d _dim, Vec2d _index) {
		ID    = _ID;
		cs    = _cs;
		tau   = _tau;
		dim   = _dim;
		index = _index;

		//Set neighbor node
		for (int k = 0; k < Q; k++) {
			aux[0] = (int)index[0] + cx[k];
			aux[1] = (int)index[1] + cy[k];
			if (aux[0] == -1)	aux[0] = dim[0] - 1;
			if (aux[1] == -1)	aux[1] = dim[1] - 1;

			if (aux[0] == (int)dim[0])	aux[0] = 0;
			if (aux[1] == (int)dim[1])	aux[1] = 0;

			nCell[k] = aux[0] + dim[0] * aux[1];
		}
	}

	//Methods:
	double set_eqFun(double _rho, Vec2d _vel, int k);

	//Cell variables:
	Vec2d  aux;
	Vec2d  dim;
	Vec2d  index;
	Vec2d  vel = Vec2d::Zero();
	double rho = 0.0;
	bool   Boundary = false;
	bool   isSolid = true;
	int    ID;
	double cs;
	double tau;

	//D2Q9 Variables:
	const int Q = 9;
	const std::vector<int> cx   = { 0, 1, 0, -1, 0, 1, -1, -1, 1 };
	const std::vector<int> cy   = { 0, 0, 1, 0, -1, 1, 1, -1, -1 };
	const std::vector<int> op   = { 0, 3, 4, 1, 2, 7, 8, 5, 6 };
	const std::vector<double> w = { 4.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 1.0 / 36.0, 1.0 / 36.0, 1.0 / 36.0, 1.0 / 36.0 };
	Vec9d f      = Vec9d::Zero();
	Vec9d fTemp  = Vec9d::Zero();
	Vec9d nCell  = Vec9d::Zero();


};
#endif // !LATTICE_H
