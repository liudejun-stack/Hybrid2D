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
//	Vector2r domainSize     = { 500, 100 };
//	Vector2r cylinderCoord  = { domainSize[1] / 2., domainSize[1] / 2. };
//	double   particleRadius = domainSize[1] / 20 + 1;
//	double   uMax           = 0.1;
//	double   Re             = 5;
//
//	//Geometry
//	S.domainSize     = domainSize;
//	S.simDuration    = 15000.0;
//	S.topIsSolid     = true;
//	S.botIsSolid     = true;
//	S.bodiesAreSolid = true;
//	S.eIMB.eLBM.setVelWest = true;
//	//S.eIMB.eLBM.setDenEast = true;
//
//	//Bodies:
//	S.addCircle(1, particleRadius, cylinderCoord, { 0.0, 0.0 });
//
//	//Fluid Properties:
//	S.kinViscosity   = uMax * (2 * particleRadius) / Re;
//	S.relaxationTime = 3.0 * S.kinViscosity + 0.5;
//	S.velInit        = { 0.08, 0.0 };
//	S.rhoInit        = 1.0;
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
//		//S.eIMB.eLBM.setDenBC(domainSize[0] - 1, j, 1.0);
//		//S.eIMB.eLBM.setDenBC(0, j, 1.0);
//	}
//
//	int ignore = system("mkdir VTK_Fluid");
//	int i = 0;
//	while (S.Time < S.simDuration) {
//		if (i % 1000 == 0) {
//			Out.displaySimulationInfo();
//			//Out.fluidVelocityProfile("Entrada", 0);
//			//Out.fluidVelocityProfile("Saida", domainSize[0]-1);
//		}
//
//		S.LBMEngine();
//		if (i % 100 == 0)	Out.fluidVTK("LBM");
//
//		S.Time += S.eIMB.eLBM.dtLBM;
//		++i;
//	}
//
//	return 0;
//}