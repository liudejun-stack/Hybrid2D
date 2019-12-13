/*
#include "Scene.h"

double uMax = 0.1;
double re = 100;
Vector2r dim = { 100, 200 };
double radius = dim[0] / 20 + 1;
Vector2r cylinderCoord = { dim[0] / 2, dim[0] / 2 };


double calcVisc() {
	double kinVisc = uMax * (2 * radius) / re;
	return 3.0 * kinVisc + 0.5;
}

void calcInitSpeed(int x, int y, Vector2r& _vel) {
	double L = dim[1] - 2;
	double yp = y - 1.5;
	_vel[0] = uMax * 4 / (L * L) * (L * yp - yp * yp);
	_vel[1] = 0.0;
}

int main() {
	
	Timer Time;
	Scene S;

	//Geometry
	S.domainSize = dim;
	S.left_isSolid = true;
	S.right_isSolid = true;

	//Fluid Parameters:
	S.relaxationTime = calcVisc();
	S.rhoInit = 1.0;
	S.velInit = { 0.0, 0.08 };

	//Prepare Scenario
	S.prepareScenario();
	
	for (int i = 0; i < dim[0]; ++i) {
		Vector2r vel;
		calcInitSpeed(i, dim[1]-1, vel);
		S.eIMB.eLBM.setVelBC(i, dim[1]-1, vel);
		S.eIMB.eLBM.setDenBC(i, 0, 1.0);
	}
	S.eIMB.eLBM.setZouBC();

	for (int i = 0; i != 10000; ++i) {
		S.moveToNextTimeStep_LBM();
		if (i % 100 == 0) {
			S.fluidVTK("LBM");
			//S.solidVTK("DEM");
		}
		if (i % 1000 == 0) {
			S.eIMB.eDEM.calculateEnergy();
			S.simulationInfo(i);
		}
	}

	return 0;
}
*/