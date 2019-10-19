#ifndef LBM_H
#define LBM_H

//Standard Library
#include <memory>
#include <string>
#include <fstream>

//Hybrid Library
#include "Lattice.h"
#include "Math.h"

class LBM {
public:

	//Getters:
	int getCell(int i, int j);

	//Boundary conditions:
	void setVelBC(int i, int j, Vec2d _vel);
	void setDenBC(int i, int j, double _rho);
	void setZouBC();
	void setBounceBack();

	//LBM Engine:
	void initializeCells();
	void updateMacro();
	void setInitCond(double _rhoInit, Vec2d _vel);
	void resetSolidFraction();
	void applyForce();
	void collision();
	void stream();

	std::vector<std::shared_ptr<Lattice>> cells;

	Vec2d gravity       = { 0.0, -9.81 };
	Vec2d domainSize    = Vec2d::Zero();
	double dx           = 1.0;
	double dtLBM        = 1.0;
	double tau          = 1.0;
	double latticeSpeed = 1.0;
	double kinViscosity = 1.0;
	int isFluid         = 0;
	int isSolid         = 1;
	int vtkCounter      = 0;
};
#endif // !LBM_H

