#ifndef BODY_H
#define BODY_H

//Standard Library
#define _USE_MATH_DEFINES
#include <math.h>
#include <memory>
#include <vector>

//Hybrid Library
#include "Math.h"

class Interaction;

class Body {
public:
	
	Body(double _mass, double _radius, Vector2r _pos, Vector2r _vel, int _id, bool _fixed) {
		mass          = _mass;
		radius        = _radius;
		inertiaMoment = _mass * _radius * _radius * 0.5;
		pos           = _pos;
		vel           = _vel;
		id            = _id;
		if (_fixed) { blockedDOFs = Vector2r::Zero(); blockedMDOFs = 0; }
	}

	bool checkInteraction(int _bodyId);
	void calculateEnergy();
	bool fluidInteraction(Vector2r _cellPos, double _dx);

	//Body variables:
	double density;
	double radius;
	double mass;
	double inertiaMoment;
	int    id;

	//Smart pointers:
	std::vector<std::weak_ptr<Interaction>> inter;
	std::vector<int> fluidSolidInteraction;

	Vector2r pos         = Vector2r::Zero();
	Vector2r vel         = Vector2r::Zero();
	Vector2r force       = Vector2r::Zero();
	Vector2r forceLBM    = Vector2r::Zero();
	Vector2r blockedDOFs = { 1,1 };
	double torqueLBM     = 0.0;
	double rot           = 0.0;
	double rotVel        = 0.0;
	double moment        = 0.0;
	double blockedMDOFs  = 1;
	double kinEnergy     = 0.0;
	double potEnergy     = 0.0;

	~Body() {};
};

#endif //BODY_H