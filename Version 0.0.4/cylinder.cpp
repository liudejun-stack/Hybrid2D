/*
#include "LBM.h"

double uMax = 0.1;
double re = 5;
Vec2d dim = { 500, 100 };
double radius = dim[1] / 20 + 1;
double obsX = dim[1] / 2;
double obsY = dim[1] / 2;

double calcVisc() {
	double kinVisc = uMax * (2 * radius) / re;
	return 3.0 * kinVisc + 0.5;
}

void calcInitSpeed(int x, int y, Vec2d& _vel) {
	double L = dim[1] - 2;
	double yp = y - 1.5;
	_vel[0] = uMax * 4 / (L * L) * (L * yp - yp * yp);
	_vel[1] = 0.0;
}

int main() {
	Timer Time;
	
	LBM L;
	L.tau = calcVisc();
	L.dim = dim;
	L.initializeCells();
	L.setBotSolid();
	L.setTopSolid();
	L.setCircle({obsX,obsY}, radius);

	L.setinitCond(1.0, { 0.08, 0.0 });
	L.applyForce();
	

	for (int j = 0; j < dim[1]; j++) {
		Vec2d vel;
		calcInitSpeed(0, j, vel);
		L.setvelBC(0, j, vel);
		L.setdenBC(dim[0] - 1, j, 1.0);
	}
	L.setzouBC();
	L.solver(10000, "LBM");


	return 0;
}
*/