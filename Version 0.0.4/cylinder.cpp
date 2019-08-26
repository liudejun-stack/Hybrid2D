#include "IMB.h"


double uMax = 0.1;
double re = 100;
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
	
	double tau = calcVisc();
	IMB I(dim, 1, 1, tau);
	I.fluid.setBotSolid();
	I.fluid.setTopSolid();
	I.fluid.setCircle({obsX,obsY}, radius);

	I.fluid.setinitCond(1.0, { 0.08, 0.0 });
	I.fluid.applyForce();
	

	for (int j = 0; j < dim[1]; j++) {
		Vec2d vel;
		calcInitSpeed(0, j, vel);
		I.fluid.setvelBC(0, j, vel);
		I.fluid.setdenBC(dim[0] - 1, j, 1.0);
	}
	I.fluid.setzouBC();
	I.fluid.solver(10000, "LBM");

	return 0;
}
