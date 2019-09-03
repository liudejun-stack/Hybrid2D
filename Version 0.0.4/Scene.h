#ifndef SCENE_H
#define SCENE_H

#include "IMB.h"

class Scene {
public:
	Scene() : coupling() {};

	//Scenario prepartion:
	void addCircle(double _mass, double _radius, Vec2d _pos, Vec2d _vel);
	void set_circlesSolid();
	void set_topSolid();
	void set_botSolid();
	void set_leftSolid();
	void set_rightSolid();

	//Prepare scenario:

	IMB coupling;
	Vec2d domainSize = Vec2d::Zero();
};

#endif // !SCENE_H

