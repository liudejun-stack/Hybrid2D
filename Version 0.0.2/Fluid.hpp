#ifndef _FLUID_
#define _FLUID_

#include "D2Q9.hpp"
#include "Grid.hpp"
#include <memory>
#include <iostream>
#include <math.h>
#include <string>
#include <fstream>

class Fluid {
public:

	//Fluid constructor:
	Fluid(double _tau = 1.0, double _RhoInit = 1.0, double _uxInit = 0.0, double _uyInit = 0.0) :
		tau(_tau), RhoInit(_RhoInit), uxInit(_uxInit), uyInit(_uyInit) {};

	//Methods declaration
	double EqFun(double _Rho, double _ux, double _uy, int k);		//Equilibrium distribution function
    void InitCond();			                                    //Initialize distribution functio
	void Collision();			                                    //Apply collision
	void Stream();				                                    //Apply stream
    void BounceBack();                                              //Apply BounceBack
    void ZouHeBC();                                                 //Apply Zou & He boundary conditions
	void MacroUpdate();			                                    //Update macroscopic variables
    void writeFluidVTK(std::string _Filename);                      //Outputs VTK file for visualization
    void solve(int nIter, std::string _Filename);                   //LBM solver

	//Pointers and Vectors:
    std::shared_ptr<D2Q9> D = std::make_shared<D2Q9>();             //Smart pointer to class D2Q9
    std::shared_ptr<Grid> G = std::make_shared<Grid>();             //Smart pointer to class Grid
	std::vector<int> op = {0, 3, 4, 1, 2, 7, 8, 5, 6};				//Opposing function vector
	
	//Fluid variables:
	double tau;
	double RhoInit;
	double uxInit;
	double uyInit;
    int vtkCounter = 1;

	//Macroscopic Variables
	double rho[101 * 101];
	double ux[101 * 101];
	double uy[101 * 101];

	//Distribution Functions
	double f[101 * 101 * 9];				//f for initial condition and stream
	double fTemp[101 * 101 * 9];			//f for collision

	
	
};

#endif // !_FLUID_

