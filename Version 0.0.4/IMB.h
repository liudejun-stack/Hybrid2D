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
	void calculateForceAndTorque();

	LBM fluid;
	DEM particle;
	double dt = 0.0;

};

#endif // !SCENE_H

