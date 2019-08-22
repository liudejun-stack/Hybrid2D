#ifndef IMB_H
#define IMB_H

#include "LBM.h"
#include "DEM.h"

class IMB {
public:
	//Constructor:
	IMB(Vec2d _dim, double _dx, double _dt, double _tau) : fluid(_dim, _dx, _dt, _tau), particle(_dim) {};

	//Preparação de cenário;
	void addBody(double _mass, double _radius, Vec2d _pos, Vec2d _vel);

	//Setters
<<<<<<< HEAD
	void setTimeStep(double _FoS, double _kinematicViscosity);
=======
	void setTimeStep(double _FoS, double _maxStiffness);
>>>>>>> bc7dadff60807093e0b9aceeab8a075227273fcb
	void setSolidFraction();

	//Engine
	void calculateHydroForce();
	void solve(std::string _fileName, int _nIter);

	LBM fluid;
	DEM particle;

};

#endif // !SCENE_H

