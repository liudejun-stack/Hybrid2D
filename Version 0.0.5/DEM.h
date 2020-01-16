#ifndef DEM_H
#define DEM_H

//Standard Library
#include <vector>
#include <memory>

//Hybrid Library
#include "Math.h"
#include "Interaction.h"
#include "Body.h"

class DEM {
public:

	//Engine:
	void calculateParticleTimeStep();
	void contactVerification();
	void forceCalculation();
	void updateVelPos();
	void updateContact();
	Vector2r applyBorderForce(std::shared_ptr<Body> _body);
	void calculateEnergy();

	//Smart pointers to classes:
	std::vector<std::shared_ptr<Body>> bodies;
	std::vector<std::shared_ptr<Interaction>> interactions;

	//Vector:
	std::vector<double> kinEnergy;
	std::vector<double> potEnergy;

	//Model variables:
	Vector2r  domainReference = Vector2r::Zero();
	Vector2r  domainSize = Vector2r::Zero();
	Vector2r  gravity = { 0.0, -9.81 };
	double normalStiffness = 1e6;
	double shearStiffness = 0.5e6;
	double localDamping = 0.8;
	double frictionAngle = 30;
	double borderStifness = 1e6;
	double dtDEM = 0.0;
	double factorOfSafety = 0.1;
	int    nIter = 0;
};

#endif //DEM_H