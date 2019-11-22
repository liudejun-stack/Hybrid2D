//#include "Scene.h"
//#include "Output.h"
//
//int main() {
//
//	Timer Time;
//	Scene& S = S.getScene();
//	Output Out;
//
//	//General Information
//	Vector2r  domainSize = { 500, 100 };
//	double particleRadius = domainSize[1] / 20 + 1;
//	Vector2r  cylinderCoord = { 2*particleRadius, particleRadius };
//	double uMax = 0.1;
//	double Re = 100;
//
//	S.domainSize = domainSize;
//	S.eIMB.eLBM.setVelWest = true;
//	S.eIMB.eLBM.setDenEast = true;
//
//	//Bodies:
//	S.addCircle(1, particleRadius, cylinderCoord, Vector2r::Zero());
//
//	//Fluid Properties:
//	S.kinViscosity = uMax * (2 * particleRadius) / Re;
//	S.relaxationTime = 3.0 * S.kinViscosity + 0.5;
//	S.velInit = { 0.08, 0.0 };
//	S.rhoInit = 1.0;
//
//	//Particle Properties:
//	S.frictionAngle = 30;
//	S.localDamping = 0;
//	S.factorOfSafety = 0.1;
//	S.normalStiffness = 1.0e6;
//	S.shearStiffness = 0.5e6;
//	S.simDuration = 30000;
//
//	//Prepare Scenario
//	S.prepareScenario();
//
//	for (int j = 0; j < domainSize[1]; ++j) {
//		double L = domainSize[1] - 2;
//		double yp = j - 1.5;
//		Vector2r Vel = Vector2r::Zero();
//		Vel[0] = uMax * 4 / (L * L) * (L * yp - yp * yp);
//		Vel[1] = 0.0;
//		S.eIMB.eLBM.setVelBC(0, j, Vel);
//		S.eIMB.eLBM.setDenBC(domainSize[0] - 1, j, 1.0);
//	}
//
//	int ignore = system("mkdir VTK_Fluid");
//	    ignore = system("mkdir VTK_Solid");
//
//	double tlbm = 0.0;
//	int i = 0;
//	while (S.Time < S.simDuration) {
//		if (i % 1000 == 0)	Out.displaySimulationInfo();
//		S.eIMB.defineLinkedCells();
//		S.eIMB.calculateForceAndTorque();
//		S.DEMEngine();
//		if (S.Time >= tlbm) {
//			S.LBMEngine();
//			tlbm += S.eIMB.eLBM.dtLBM;
//		}
//		S.eIMB.eLBM.resetSolidFraction();
//		if (i % 1000 == 0) {
//			Out.fluidVTK("LBM");
//			Out.solidVTK("DEM");
//		}
//		S.Time += S.eIMB.eDEM.dtDEM;
//		++i;
//	}
//
//	return 0;
//}