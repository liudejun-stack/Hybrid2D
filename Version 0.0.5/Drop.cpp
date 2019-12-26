//#include "Scene.h"
//#include "Output.h"
//
//int main() {
//	Scene& S = S.getScene();
//	Output Out;
//	
//	//Geometry
//	S.domainSize = { 10, 10 };
//	S.addCircle(1.0, 0.5, { 2, 0.5 }, { 4.0, 0.0 }, false);
//	S.addCircle(1.0, 0.5, { 5, 0.5 }, { 0.0, 0.0 }, false);
//
//	//Solid Properties:
//	S.frictionAngle = 30;
//	S.localDamping = 0.7;
//	S.factorOfSafety = 0.1;
//	S.normalStiffness = 1.0e6;
//	S.shearStiffness = 0.5e6;
//	S.simDuration = 100000;
//	
//	//Solver
//	S.prepareScenario();
//	int ignore = system("mkdir VTK_Solid");
//	for (int i = 0; i != S.simDuration; ++i) {
//		if (i % 1000 == 0) {
//			Out.displaySimulationInfo();
//		}
//		if (i % 1000 == 0) {
//			Out.solidVTK("DEM");
//			Out.particleEnergy("Body1.csv", 0);
//			Out.particleEnergy("Body2.csv", 1);
//		}
//		S.DEMEngine();
//	}
//
//	return 0;
//}