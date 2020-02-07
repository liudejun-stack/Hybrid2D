#ifndef LATTICE_H
#define LATTICE_H

//Standard Libray
#include <vector>

//Hybrid Library
#include "Math.h"

class Lattice {
public:
	//Constructor:
	Lattice(int _ID, double _dx, double _dt, double _tau, Vector2r _dim, Vector2r _cellPos) {
		ID            = _ID;
		dx            = _dx;
		dt            = _dt; 
		tau           = _tau;
		latticeSpeed  = _dx / _dt;
		dim           = _dim;
		cellPos       = _cellPos;

		//Set neighbor node
		for (int k = 0; k < Q; k++) {
			aux = cellPos + discreteVelocity[k];
			
			if (aux[0] == -1)	aux[0] = dim[0] - 1;
			if (aux[1] == -1)	aux[1] = dim[1] - 1;

			if (aux[0] == (int)dim[0])	aux[0] = 0;
			if (aux[1] == (int)dim[1])	aux[1] = 0;

			nCell.push_back(aux[0] + dim[0] * aux[1]);
		}
	}

	//Methods:
	double setEqFun(double _rho, Vector2r _vel, int k);
	double setSourceTerm(double& _tau, double _dt, int k);
	double getDensityAndVelocity(Vector2r& _vel);
	void updateMacro();

	friend class LBM;
	friend class IMB;

	//Cell variables:
	int       ID;                  //Cell ID               
	double    dx;                  //Lattice Spacing
	double    dt;                  //Time step
	double    tau;                 //Relaxation Time
	double    latticeSpeed;        //Speed of the model
	double    rhoBC;               //Prescribed Density for Zou & He Boundary Condition
	Vector2r  velBC;               //Prescribed Velocity for Zou & He Boundary Condition
	Vector2r  aux;                 //Auxiliar vector to calculate neighbour node
	Vector2r  dim;                 //Domain size
	Vector2r  cellPos;             //Cell position

	Vector2r sourceForce           = Vector2r::Zero();
	Vector2r vel                   = Vector2r::Zero();
	double   solidFraction         = 0.0;
	double   previousSolidFraction = 0.0;
	double   rho                   = 0.0;
	int      particleFluidID       = 0;
	int      node                  = 0;
	int      isSolid               = 1;
	int      isFluid               = 0;

private:
	//D2Q9 Variables:
	const int Q = 9;
	const std::vector<int>      opNode           = { 0, 3, 4, 1, 2, 7, 8, 5, 6 };
	const std::vector<double>   weight           = { 4.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 1.0 / 36.0, 1.0 / 36.0, 1.0 / 36.0, 1.0 / 36.0 };
	const std::vector<Vector2r> discreteVelocity = { {0,0}, {1,0}, {0,1}, {-1,0}, {0,-1}, {1,1}, {-1,1}, {-1,-1}, {1,-1} };
	
	std::vector<double> f     = { 0,0,0,0,0,0,0,0,0 };
	std::vector<double> fTemp = { 0,0,0,0,0,0,0,0,0 };
	std::vector<double> omega = { 0,0,0,0,0,0,0,0,0 };
	std::vector<double> nCell;
};

#endif // !LATTICE_H