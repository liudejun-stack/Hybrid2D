#ifndef DEM_H
#define DEM_H

#include <string>
#include <fstream>
#include "Interaction.h"


class DEM {
public:

	//Constructor:
	DEM(Vec2d _domainSize) : domainSize(_domainSize) {};

	//Engine:
	Vec2d applyBorderForce(std::shared_ptr<Body> _body);
	void  demEnergy();
	void  demCycle();

	//Output:
	void  outputECSV(std::string _fileName);
	void  outputSVTK(std::string _fileName);

	//Smart pointers to classes:
	std::vector<std::shared_ptr<Body>> bodies;
	std::vector<std::shared_ptr<Interaction>> interactions;

	//Vector:
	std::vector<double> kinEnergy;
	std::vector<double> potEnergy;

	//Model variables:
	Vec2d  domainSize;
	Vec2d  domainReference = Vec2d::Zero();
	Vec2d  gravity = { 0.0, -9.81 };
	double kn = 1e6;
	double ks = 0.5e6;
	double localDamping = 0.8;
	double frictionAngle = 30;
	double borderStifness = 1e6;
	double dt = 0.0;
	double time = 0.0;
	int    vtkCounter = 0;
	int    nIter = 0;
};

#endif //DEM_H