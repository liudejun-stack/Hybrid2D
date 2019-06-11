#ifndef FLUID_H
#define FLUID_H
#include "Lattice.hpp"
#include <memory>
#include <string>
#include <fstream>

class Fluid {
public:

	//Fluid constructor:
	Fluid(double _nu, Vec3i _nDim, double _dx, double _dt) {
		nu = _nu;
		nDim = _nDim;
		dx = _dx;
		dt = _dt;
		cs = dx / dt;
		tau = 3.0*nu*dt / (dx*dx) + 0.5;
        Ncells = nDim[0]*nDim[1]*nDim[2];
		
		for (int k = 0; k < nDim[2]; k++)
		for (int j = 0; j < nDim[1]; j++)
		for (int i = 0; i < nDim[0]; i++) {
			Vec3i Aux = { i,j,k };
			c.push_back(std::make_shared<Lattice>(Aux, nDim, cs, tau));
		}
	}

	//Methods:
	int GetCell(int i, int j);
	void defGeom(bool _top, bool _bot, bool _left, bool _right);
	void setObstacle(int _obsX, int _obsY, int _radius);
	void setDensBC(int i, int j, double _rho);
	void setVelBC(int i, int j, Vec3d& _vel);
	void Collision();
	void BounceBack();
	void Stream();
	void writeFVTK(std::string _filename);
	void solve(int nIter, std::string _filename);

	//Variables:
	double nu;
	double dx;
	double dt;
	double cs;
	double tau;
	Vec3i nDim;
	double Ncells;
	std::vector<std::shared_ptr<Lattice>> c;
	int vtkCounter = 1;
};
#endif // !_FLUID_H
