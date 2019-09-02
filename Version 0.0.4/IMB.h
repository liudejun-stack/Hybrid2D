#ifndef IMB_H
#define IMB_H

#include "LBM.h"
#include "DEM.h"

class IMB {
public:
	//Constructor:
	IMB() : fluid(), particle() {};

	//Preparação de cenário;
	void addBody(double _mass, double _radius, Vec2d _pos, Vec2d _vel);

	//Setters
	void setSolidFraction();

	//Engine
	void calculateHydroForce();
	void solve(std::string _fileName, int _nIter);

	LBM fluid;
	DEM particle;

};

#endif // !SCENE_H

