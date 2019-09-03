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

	//Domain:
	void setCircle(Vec2d _center, double _radius);
	void setSquare(Vec2d _initPos, double _squareSide);

	//Boundary conditions:
	void setvelBC(int i, int j, Vec2d _vel);
	void setdenBC(int i, int j, double _rho);
	void setzouBC();
	void bounceback();

	//LBM Engine:
	void initializeCells();
	void calculateFluidTimeStep();
	void setinitCond(double _rhoInit, Vec2d _vel);
	void updateMacro();
	void applyForce();
	void collision();
	void stream();
	void solver(int _nIter, std::string _fileName);

	//Coupling Engine:
	void c_collision();

	//Output:
	void fluidVTK(std::string _fileName);

	std::vector<std::shared_ptr<Lattice>> cells;

	Vec2d gravity       = { 0.0, -9.81 };
	Vec2d dim           = Vec2d::Zero();
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

