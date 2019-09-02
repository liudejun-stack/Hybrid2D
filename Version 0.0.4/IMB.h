#ifndef IMB_H
#define IMB_H

#include "LBM.h"
#include "DEM.h"

class IMB {
public:
	//Constructor:
	IMB() : fluid(), particle() {};

	//Engine
	void calculateTimeStep();
	void calculateSolidFraction();
	void calculateForceAndTorque();

	LBM fluid;
	DEM particle;
	double dt;

};

#endif // !SCENE_H

