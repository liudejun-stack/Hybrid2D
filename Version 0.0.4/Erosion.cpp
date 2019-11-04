#include "Scene.h"

int main() {

	Timer Time;
	Scene S;

	//General Information
	Vec2d  domainSize = { 500, 100 };
	double particleRadius = domainSize[1] / 20 + 1;
	Vec2d  cylinderCoord = { particleRadius, particleRadius };
	double uMax = 0.1;
	double Re = 100;

	S.domainSize = domainSize;

	//Bodies:
	S.addCircle(1, particleRadius, cylinderCoord, Vec2d::Zero());

	//Fluid Properties:
	S.kinViscosity = uMax * (2 * particleRadius) / Re;
	S.relaxationTime = 3.0 * S.kinViscosity + 0.5;
	S.velInit = { 0.08, 0.0 };
	S.rhoInit = 1.0;

	//Particle Properties:
	S.frictionAngle = 30;
	S.localDamping = 0;
	S.factorOfSafety = 0.1;
	S.normalStiffness = 1.0e6;
	S.shearStiffness = 0.5e6;
	S.simDuration = 30000;

	//Prepare Scenario
	S.prepareScenario();
	S.eIMB.eLBM.setInitCond(S.rhoInit, S.velInit);

	S.moveToNextTimeStep();
	
	return 0;
}