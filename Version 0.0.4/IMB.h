#ifndef IMB_H
#define IMB_H

#include "LBM.h"
#include "DEM.h"

class IMB {
public:
	//Constructor:
	IMB() : eLBM(), eDEM() {};

	//Engine
	double calculateSolidFraction(Vec2d& _particlePos, Vec2d& _cellPos, double _particleRadius, double _dx);
	void calculateForceAndTorque();

	LBM eLBM;	//LBM engine
	DEM eDEM;	//DEM engine

};

#endif // !SCENE_H

