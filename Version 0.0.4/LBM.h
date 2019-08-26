#ifndef LBM_H
#define LBM_H

#include "Lattice.h"
#include <memory>
#include <string>
#include <fstream>

class LBM {
public:
	//Constructor:
	LBM(Vec2d _dim, double _dx, double _dt, double _tau) {
		dim          = _dim;
		Ncells       = _dim[0] * _dim[1];
		dx           = _dx;
		dtLBM        = _dt;
		tau          = _tau;
		tauInv       = 1.0 / _tau;
		latticeSpeed = _dx / _dt;

		for (int j = 0; j < _dim[1]; j++)
		for (int i = 0; i < _dim[0]; i++) {
			Vec2d cellPos = { i,j };
			int id = cells.size();
			cells.emplace_back(std::make_shared<Lattice>(id,latticeSpeed, dim, cellPos));
		}
	}

	//Getters:
	int getCell(int i, int j);

	//Domain:
	void setCircle(Vec2d _center, double _radius);
	void setSquare(Vec2d _initPos, double _squareSide);
	void setTopSolid();
	void setBotSolid();
	void setRightSolid();
	void setLeftSolid();

	//Boundary conditions:
	void setvelBC(int i, int j, Vec2d _vel);
	void setdenBC(int i, int j, double _rho);
	void setzouBC();
	void bounceback();

	//Output:
	void fluidVTK(std::string _fileName);

	//LBM Engine:
	void calculateFluidTimeStep();
	void setinitCond(double _rhoInit, Vec2d _vel);
	void updateMacro();
	void applyForce();
	void collision();
	void stream();
	void solver(int _nIter, std::string _fileName);

	//Coupling Engine:
	void c_collision();

	std::vector<std::shared_ptr<Lattice>> cells;

	Vec2d dim;
	double dx;
	double dtLBM;
	double tau;
	double Ncells;
	double latticeSpeed;
	double tauInv;
	double kinViscosity;

	Vec2d gravity    = { 0.0, -9.81 };
	bool  isFluid    = false;
	bool  isSolid    = true;
	int   vtkCounter = 0;

};
#endif // !LBM_H

