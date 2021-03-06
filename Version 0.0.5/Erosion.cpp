#include "Scene.h"
#include "Output.h"

int main() {

	Timer Time;
	Scene& S = S.getScene();
	Output Out;

	//General Information
	Vector2r  domainSize = { 500, 100 };
	double particleRadius = domainSize[1] / 20 ;
	Vector2r  cylinderCoord = { particleRadius, 30 };
	double uMax = 0.1;
	double Re = 100;

	S.domainSize = domainSize;

	//Bodies:
	S.addCircle(1, particleRadius, cylinderCoord, Vector2r::Zero(), false);

	//Fluid Properties:
	S.kinViscosity = uMax * (2 * particleRadius) / Re;
	S.relaxationTime = 3.0 * S.kinViscosity + 0.5;
	S.velInit = { 0.0, 0.0 };
	S.rhoInit = 1.0;
	//S.topIsSolid     = true;
	//S.botIsSolid     = true;
	//S.eIMB.eLBM.setVelWest = true;
	//S.eIMB.eLBM.setDenEast = true;

	//Particle Properties:
	S.frictionAngle = 30;
	S.localDamping = 0.1;
	S.factorOfSafety = 0.1;
	S.normalStiffness = 1.0e6;
	S.shearStiffness = 0.5e6;
	S.simDuration = 10000;

	//Prepare Scenario
	S.prepareScenario();

	//for (int j = 0; j < domainSize[1]; ++j) {
	//	double L = domainSize[1] - 2;
	//	double yp = j - 1.5;
	//	Vector2r Vel = Vector2r::Zero();
	//	/*Vel[0] = uMax * 4 / (L * L) * (L * yp - yp * yp);*/
	//	Vel[0] = 0.1;
	//	Vel[1] = 0.0;
	//	S.eIMB.eLBM.setVelBC(0, j, Vel);
	//	S.eIMB.eLBM.setDenBC(domainSize[0] - 1, j, 1.0);
	//}

	int ignore = system("mkdir VTK_Fluid");
	    ignore = system("mkdir VTK_Solid");

	double tlbm = 0.0;
	int i = 0;
	while (S.Time < S.simDuration) {
		if (i % 10000 == 0)	Out.displaySimulationInfo();
		S.eIMB.eLBM.resetSolidFraction();
		S.eIMB.defineLinkedCells();
		S.eIMB.calculateForceAndTorque();
		S.DEMEngine();
		if (S.Time >= tlbm) {
			S.eIMB.eLBM.setZouBC();
			S.eIMB.eLBM.collisionNT();
			S.eIMB.eLBM.setBounceBack();
			S.eIMB.eLBM.stream();
			tlbm += S.eIMB.eLBM.dtLBM;
		}
		if (i % 10000 == 0) {
			Out.fluidVTK("LBM");
			Out.solidVTK("DEM_" + std::to_string(S.Time));
			Out.particleInfo("bodyInfo.csv", 0);
		}
		S.Time += S.eIMB.eDEM.dtDEM;
		++i;
	}

	return 0;
}