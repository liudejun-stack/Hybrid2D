#ifndef BODY_H
#define BODY_H

//Standard Library
#include <memory>
#include <vector>

//Hybrid Library
#include "Math.h"

class Interaction;

class Body {
public:
	
	Body(double _mass, double _radius, Vec2d _pos, Vec2d _vel, int _id) {
		mass          = _mass;
		radius        = _radius;
		pos           = _pos;
		vel           = _vel;
		id            = _id;
		inertiaMoment = _mass * _radius * _radius * 0.5;
	}

	bool checkInteraction(int _bodyId);

	//Body variables:
	double mass;
	double radius;
	double functionR;
	double inertiaMoment;
	int    id;

	//Smart pointers:
	std::vector<std::weak_ptr<Interaction>> inter;

	//Vectors:
	Vec2d pos         = Vec2d::Zero();
	Vec2d vel         = Vec2d::Zero();
	Vec2d force       = Vec2d::Zero();
	Vec2d forceLBM    = Vec2d::Zero();
	Vec2d torqueLBM   = Vec2d::Zero();
	Vec2d blockedDOFs = { 1,1 };

	//Body motion variables:
	double rot          = 0.0;
	double rotVel       = 0.0;
	double moment       = 0.0;
	double blockedMDOFs = 1;

	~Body() {};
};

#endif //BODY_H