//#include "Scene.h"
//
//int main() {
//	
//	Timer Time;
//	Scene S;
//
//	//General Information
//	Vec2d  domainSize     = { 500, 100 };
//	Vec2d  cylinderCoord  = { domainSize[1] / 2, domainSize[1] / 2 };
//	double particleRadius = domainSize[1] / 20 + 1;
//	double uMax           = 0.1;
//	double Re             = 100;
//
//	//Geometry
//	S.domainSize     = domainSize;
//	S.topIsSolid     = true;
//	S.botIsSolid     = true;
//	S.bodiesAreSolid = true;
//	S.eIMB.eLBM.setVelWest = true;
//	S.eIMB.eLBM.setDenEast = true;
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
//	S.eIMB.eLBM.setInitCond(S.rhoInit, S.velInit);
//	
//	for (int j = 0; j < domainSize[1]; ++j) {
//		double L = domainSize[1] - 2;
//		double yp = j - 1.5;
//		Vec2d Vel = Vec2d::Zero();
//		Vel[0] = uMax * 4 / (L * L) * (L * yp - yp * yp);
//		Vel[1] = 0.0;
//		S.eIMB.eLBM.setVelBC(0, j, Vel);
//		S.eIMB.eLBM.setDenBC(domainSize[0] - 1, j, 1.0);
//	}
//
//	S.LBMSolver();
//
//	return 0;
//}