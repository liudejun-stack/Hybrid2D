#ifndef SCENE_H
#define SCENE_H

//Hybrid Libary
#include "IMB.h"
#include "Math.h"
#include "Timer.h"

class Scene {
public:
	Scene() : eIMB() {};

	//Scenario prepartion:
	void addCircle(double _mass, double _radius, Vec2d _pos, Vec2d _vel);
	void setBodiesSolid();
	void setTopSolid();
	void setBotSolid();
	void setLeftSolid();
	void setRightSolid();
	void prepareScenario();
	void simulationInfo(int& i);

	//Solver:
	void LBMSolver();
	void DEMSolver();
	void moveToNextTimeStep();

	//Outputs
	void fluidVTK(std::string _fileName);
	void solidVTK(std::string _fileName);

	//Geometry parameters:
	Vec2d domainSize       = Vec2d::Zero();
	bool top_isSolid       = false;
	bool bot_isSolid       = false;
	bool left_isSolid      = false;
	bool right_isSolid     = false;
	bool bodies_areSolid   = false;
	
	//Fluid parameters:
	double latticeSpacing  = 1.0;
	double kinViscosity    = 1e-6;
	double relaxationTime  = 1.0;
	double rhoInit         = 1.0;
	Vec2d  velInit         = { 0.08, 0.0 };

	//Particle parameters:
	double frictionAngle   = 30;
	double localDamping    = 0.0;
	double factorOfSafety  = 0.3;
	double normalStiffness = 1e6;
	double shearStiffness  = 0.5e6;

	int simDuration = 10000;
	double Time = 0.0;
	int subCycleNumber = 0;

	//Output parameters:
	int fluidVtkCounter;
	int particleVtkCounter;

	IMB eIMB;  //Coupling Engine
};

#endif // !SCENE_H

