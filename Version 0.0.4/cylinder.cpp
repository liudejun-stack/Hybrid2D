#include "Scene.h"

double uMax = 0.1;
double re = 100;
Vec2d dim = { 500, 100 };
double radius = dim[1] / 20 + 1;
Vec2d cylinderCoord = { dim[1] / 2, dim[1] / 2 };


double calcVisc() {
	double kinVisc = uMax * (2 * radius) / re;
	return 3.0 * kinVisc + 0.5;
}

void calcInitSpeed(int x, int y, Vec2d& _vel) {
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
	S.topIsSolid = true;
	S.botIsSolid = true;
	S.bodiesAreSolid = true;

	//Bodies:
	S.addCircle(1, radius, { radius, radius }, { 0.0, 0.0 });

	//Fluid Parameters:
	S.relaxationTime = calcVisc();
	S.velInit = { 0.08, 0.0 };
	S.rhoInit = 1.0;

	//Prepare Scenario
	S.prepareScenario();
	
	for (int j = 0; j < dim[1]; ++j) {
		Vec2d vel;
		calcInitSpeed(0, j, vel);
		S.eIMB.eLBM.setVelBC(0, j, vel);
		S.eIMB.eLBM.setDenBC(dim[0] - 1, j, 1.0);
	}
	S.eIMB.eLBM.setInitCond(S.rhoInit, S.velInit);
	S.eIMB.eLBM.setZouBC();

	//S.LBMSolver();
	S.moveToNextTimeStep();

	return 0;
}
