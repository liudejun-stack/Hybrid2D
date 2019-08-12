#ifndef SCENE_H
#define SCENE_H

#include "LBM.h"
#include "DEM.h"

class Scene {
public:
	//Constructor:
<<<<<<< HEAD
	Scene(Vec2d _dim, double _dx, double _dt, double _tau) : fluid(_dim, _dx, _dt, _tau), particle(_dim), dx(_dx) {};
=======
	Scene(Vec2d _dim, double _dx, double _dt, double _tau) : L(_dim, _dx, _dt, _tau), D(_dim) {};
>>>>>>> f2f35b365bc86032c55e2f1f3dbd40ff2c2ddf3c
	
	//Preparação de cenário;
	void addBody(double _mass, double _radius, Vec2d _pos, Vec2d _vel);
	void setTimeStep(double _FoS, double _maxStiffness);
	void lbmForce();


	double dx;
	LBM fluid;
	DEM particle;

	void arroz();
};

#endif // !SCENE_H

