#ifndef IMB_H
#define IMB_H

#include "LBM.h"
#include "DEM.h"

class IMB {
public:
	//Constructor:
	IMB() : eLBM(), eDEM() {};

	//Engine
	void calculateTimeStep();
	void calculateForceAndTorque();

	std::vector<double>solidFunction;
	double dt = 0.0;


	LBM eLBM;	//LBM engine
	DEM eDEM;	//DEM engine

};

#endif // !SCENE_H

