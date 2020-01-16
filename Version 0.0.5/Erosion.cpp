#include "Scene.h"
#include "Output.h"

int main() {

	Timer Time;
	Scene& S = S.getScene();
	Output Out;

	//General Information
	Vector2r  domainSize = { 500, 100 };
	double particleRadius = domainSize[1] / 20 + 1;
	Vector2r  cylinderCoord = { particleRadius, particleRadius };
	double uMax = 0.1;
	double Re = 100;

	S.domainSize = domainSize;

	//Bodies:
	S.addCircle(1, particleRadius, cylinderCoord, Vector2r::Zero(), true);

	//Fluid Properties:
	S.kinViscosity = uMax * (2 * particleRadius) / Re;
	S.relaxationTime = 3.0 * S.kinViscosity + 0.5;
	S.velInit = { 0.1, 0.0 };
	S.rhoInit = 1.0;

	//Particle Properties:
	S.frictionAngle = 30;
	S.localDamping = 0.1;
	S.factorOfSafety = 0.1;
	S.normalStiffness = 1.0e6;
	S.shearStiffness = 0.5e6;
	S.simDuration = 10000;

	//Prepare Scenario
	S.prepareScenario();

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
			Out.solidVTK("DEM");
			Out.particleInfo("bodyInfo.csv", 0);
		}
		S.Time += S.eIMB.eDEM.dtDEM;
		++i;
	}

	return 0;
}