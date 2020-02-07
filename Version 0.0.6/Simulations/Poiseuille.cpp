//#include "Scene.h"
//#include "Timer.h"
//#include "Output.h"
//
//int main() {
//	
//	Timer Time;
//	Scene& S = S.getScene();
//	Output out;
//
//	//Geometry
//	S.domainSize = { 500 , 100 };
//	S.botIsSolid = true;
//	S.topIsSolid = true;
//	S.eIMB.eLBM.setVelWest = true;
//	S.eIMB.eLBM.setDenWest = true;
//
//	//Fluid Properties
//	double Re = 5;
//	double uMax = 0.1;
//	S.relaxationTime = 1.0;
//	S.kinViscosity = (S.relaxationTime - 0.5) / 3;
//	S.velInit = { 0.08, 0.0 };
//	S.rhoInit = 1.0;
//
//	S.prepareScenario();
//
//	for (int j = 0; j < S.domainSize[1]; ++j) {
//		double L = S.domainSize[1] - 2;
//		double yp = j - 1.5;
//		Vector2r Vel = Vector2r::Zero();
//		Vel[0] = uMax * 4 / (L * L) * (L * yp - yp * yp);
//		Vel[1] = 0.0;
//		S.eIMB.eLBM.setVelBC(0, j, Vel);
//		S.eIMB.eLBM.setDenBC(S.domainSize[0] - 1, j, 1.0);
//	}
//
//	int ignore = system("mkdir VTK_Fluid");
//	int i = 0;
//	while (S.Time < S.simDuration) {
//		if (i % 1000 == 0) {
//			out.displaySimulationInfo();
//		}
//
//		S.LBMEngine();
//		if (i % 100 == 0) {
//			out.fluidVTK("LBM");
//			for (int j = 0; j < S.domainSize[1] - 1; ++j) {
//				out.fluidVelocityProfile("inlet.csv", S.eIMB.eLBM.getCell(0, j));
//				out.fluidVelocityProfile("mid.csv", S.eIMB.eLBM.getCell(S.domainSize[0] / 2, j));
//				out.fluidVelocityProfile("Outlet.csv", S.eIMB.eLBM.getCell(S.domainSize[0], j));
//			}
//		}
//
//		S.Time += S.eIMB.eLBM.dtLBM;
//		++i;
//	}
//
//
//	return 0;
//}
