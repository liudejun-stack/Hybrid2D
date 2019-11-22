#ifndef IMB_H
#define IMB_H

//Hybrid Libary
#include "Math.h"
#include "LBM.h"
#include "DEM.h"

class IMB {
public:
	//Constructor:
	IMB() : eLBM(), eDEM() {};

	//Engine
	void   defineLinkedCells();
	double calculateSolidFraction(Vector2r& _particlePos, Vector2r& _cellPos, double _particleRadius, double _dx);
	void   calculateForceAndTorque();

	LBM eLBM;	//LBM engine
	DEM eDEM;	//DEM engine

};

#endif // !SCENE_H

