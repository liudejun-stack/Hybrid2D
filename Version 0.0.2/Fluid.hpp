#ifndef FLUID_H
#define FLUID_H

#include "Lattice.hpp"
#include <memory>
#include <iostream>
#include <math.h>
#include <string>
#include <fstream>

class Fluid {
public:
    
	//Set methods:
    void initVar();
	void   setTau(double _tau);
    double setEqFun(double _rho, double _ux, double _uy, int k);				//Equilibrium distribution function
	void   setInitCond(double _rhoInit, double _uxInit, double _uyInit);		//Initialize distribution functio
	void   setVelBC(int i, int j, double _ux, double _uy);					    //Sets velocity boundary condition
	void   setDensBC(int i, int j, double _rho);								//Sets density boundary condition
	void   setObstacle(int _obsX, int _obsY, int _radius);					    //Sets obstacle
    
	//Engine declaration:
    void MacroUpdate();
	void Collision();			                                    		    //Apply collision
	void Stream();				                                    		    //Apply stream
    void BounceBack();                                              		    //Apply Bounce-Back
    void ZouHeBC();                                                 		    //Apply Zou & He boundary conditions
    void writeFluidVTK(std::string _Filename);                      		    //Outputs VTK file for visualization
    void solve(int nIter, std::string _Filename);                   		    //LBM solver

	//Pointers and Vectors:
    std::shared_ptr<Lattice> D = std::make_shared<Lattice>();       //Smart pointer to class Lattice
	std::vector<int> op = {0, 3, 4, 1, 2, 7, 8, 5, 6};				//Opposing node on bounce-back 
	
	//Variables:
	double tau;
    int vtkCounter = 1;
    bool isSolid = true;
	bool notSolid = false;

	//Macroscopic Variables
    std::vector<double> rho;
    std::vector<double> ux;
    std::vector<double> uy;

	//Distribution Functions
	std::vector<double> f;
    std::vector<double> fTemp;
};

#endif // !_FLUID_

