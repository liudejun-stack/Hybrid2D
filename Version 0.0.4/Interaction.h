#ifndef INTERACTION_H
#define INTERACTION_H

//Hybrid Libary
#define _USE_MATH_DEFINES
#include <math.h>
#include <memory>

//Hybrid Libary
#include "Math.h"

class Body;

class Interaction {
public:
	Interaction(std::weak_ptr<Body> _body1, std::weak_ptr<Body> _body2) : body1(_body1), body2(_body2) {};

	//Methods:
	bool checkContact();
	void calculateUnitVectorandContact();
	void calculateForceAndShearIncrements(double _dt, double _kn, double _ks);
	void applyFrictionLaw(double _phi);

	//Smart pointers:
	std::weak_ptr<Body> body1;
	std::weak_ptr<Body> body2;

	//Variables:
	Vec2d  unitNormal  = Vec2d::Zero();
	Vec2d  unitShear   = Vec2d::Zero();
	Vec2d  contact     = Vec2d::Zero();
	double normalForce = 0.0;
	double shearForce  = 0.0;
};

#endif //INTERACTION_H