
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
	//S.top_isSolid = true;
	//S.bot_isSolid = true;
	//S.bodies_areSolid = true;
	S.Tf = 40000.0;

	//Bodies:
	S.addCircle(1, radius, cylinderCoord, {0.0, 0.0});

	//Fluid Parameters:
	S.relaxationTime = calcVisc();

	//Particle Parameters:
	S.localDamping = 0;
	S.factorOfSafety = 0.5;
	S.frictionAngle = 30;
	S.normalStiffness = 1e6;
	S.shearStiffness = 0.5e6;

	//Prepare Scenario
	S.prepareScenario();
	S.eIMB.eDEM.dtDEM = 0.01;
	
	for (int j = 0; j < dim[1]; ++j) {
		Vec2d vel;
		calcInitSpeed(0, j, vel);
		S.eIMB.eLBM.setVelBC(0, j, vel);
		S.eIMB.eLBM.setDenBC(dim[0] - 1, j, 1.0);
	}
	S.eIMB.eLBM.setZouBC();
	
	/*
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
	*/

	S.moveToNextTimeStep();
	return 0;
}
