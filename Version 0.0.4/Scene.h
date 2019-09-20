#ifndef SCENE_H
#define SCENE_H

#include "IMB.h"

class Scene {
public:
	Scene() : eIMB() {};

	//Scenario prepartion:
	void addCircle(double _mass, double _radius, Vec2d _pos, Vec2d _vel);
	void set_circlesSolid();
	void set_topSolid();
	void set_botSolid();
	void set_leftSolid();
	void set_rightSolid();
	void updateGeom();
	void prepareScenario();
	void simulationInfo(int i);

	//Solver:
	void moveToNextTimeStep_LBM(int _nIter, std::string _fileName);
	void moveToNextTimeStep_DEM(int _nIter, std::string _fileName);

	//Geometry parameters:
	Vec2d domainSize       = Vec2d::Zero();
	bool top_isSolid       = false;
	bool bot_isSolid       = false;
	bool left_isSolid      = false;
	bool right_isSolid     = false;
	bool bodies_isSolid    = false;
	
	//Fluid parameters:
	double kinViscosity    = 1e-6;
	double relaxationTime  = 1.0;

	//Particle parameters:
	double frictionAngle   = 30;
	double localDamping    = 0.0;
	double factorOfSafety  = 0.3;
	double normalStiffness = 1e6;
	double shearStiffness  = 0.5e6;

	IMB eIMB;  //Coupling Engine
};

#endif // !SCENE_H

