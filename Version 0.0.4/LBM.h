#ifndef LBM_H
#define LBM_H

#include "Lattice.h"
#include <memory>
#include <string>
#include <fstream>

class LBM {
public:

	//Getters:
	int getCell(int i, int j);

	//Boundary conditions:
	void set_velBC(int i, int j, Vec2d _vel);
	void set_denBC(int i, int j, double _rho);
	void set_zouBC();
	void set_bounceback();

	//LBM Engine:
	void initializeCells();
	void calculateFluidTimeStep();
	void set_initCond(double _rhoInit, Vec2d _vel);
	void updateMacro();
	void applyForce();
	void collision();
	void stream();
	void solver(int _nIter, std::string _fileName);

	//Output:
	void fluidVTK(std::string _fileName);

	std::vector<std::shared_ptr<Lattice>> cells;

	Vec2d gravity       = { 0.0, -9.81 };
	Vec2d domainSize           = Vec2d::Zero();
	double dx           = 1.0;
	double dtLBM        = 1.0;
	double tau          = 1.0;
	double latticeSpeed = 1.0;
	double kinViscosity = 1.0;
	int isFluid = 0;
	int isSolid = 1;
	int fluidSolidInteraction = 2;
	int vtkCounter      = 0;

};
#endif // !LBM_H

