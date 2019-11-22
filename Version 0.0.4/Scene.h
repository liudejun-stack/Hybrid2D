#ifndef SCENE_H
#define SCENE_H

//Standard Libary
#include <iomanip>

//Hybrid Libary
#include "IMB.h"
#include "Math.h"
#include "Timer.h"

class Scene {
public:
	
	//Scene constructor:
	Scene() : eIMB() {};

	//Classes
	IMB eIMB;

	//Scenario prepartion:
	void addCircle(double _mass, double _radius, Vector2r _pos, Vector2r _vel);
	void setDomain();
	void prepareScenario();

	//Solver:
	void LBMEngine();
	void DEMEngine();

	//Method to acess the class Scene
	Scene& getScene();

	//Geometry parameters:
	Vector2r domainSize       = Vector2r::Zero();
	bool topIsSolid        = false;
	bool botIsSolid        = false;
	bool leftIsSolid       = false;
	bool rightIsSolid      = false;
	bool bodiesAreSolid    = false;
	
	//Fluid parameters:
	double latticeSpacing  = 1.0;
	double kinViscosity    = 1e-6;
	double relaxationTime  = 1.0;
	double rhoInit         = 1.0;
	Vector2r  velInit         = { 0.08, 0.0 };

	//Particle parameters:
	double frictionAngle   = 30;
	double localDamping    = 0.0;
	double factorOfSafety  = 0.3;
	double normalStiffness = 1e6;
	double shearStiffness  = 0.5e6;

	double Time            = 0.0;
	int    simDuration     = 10000;
	int    subCycleNumber  = 0;
};

#endif // !SCENE_H

