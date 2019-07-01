#ifndef LATTICE_H
#define LATTICE_H

#include "Math.h"
#include <vector>


class Lattice {
public:
	//Constructor:
	Lattice(int _ID, double _cs, double _tau) : ID(_ID), cs(_cs), tau(_tau) {};

	//Methods:
	double set_eqFun(double _rho, Vec2d _vel, int k);

	//Cell variables:
	Vec2d  vel = Vec2d::Zero();
	double rho = 0.0;
	bool   Boundary = false;
	bool   isSolid = true;
	int    ID;
	double cs;
	double tau;

	//D2Q9 Variables:
	const int Q = 9;
	const std::vector<int> cx = { 0, 1, 0, -1, 0, 1, -1, -1, 1 };
	const std::vector<int> cy = { 0, 0, 1, 0, -1, 1, 1, -1, -1 };
	const std::vector<int> op = { 0, 3, 4, 1, 2, 7, 8, 5, 6 };
	const std::vector<double> w = { 4.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 1.0 / 36.0, 1.0 / 36.0, 1.0 / 36.0, 1.0 / 36.0 };
	Vec9d f = Vec9d::Zero();
	Vec9d fTemp = Vec9d::Zero();


};
#endif // !LATTICE_H
