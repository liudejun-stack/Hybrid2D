#include "Scene.h"

double uMax = 0.1;
double re = 5;
Vec2d dim = { 500, 100 };
int radius = (int)dim[1] / 20 + 1;
int obsX = (int)dim[1] / 2;
int obsY = (int)dim[1] / 2;

double calcVisc() {
	double kinVisc = uMax * (int)(2 * radius) / re;
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
	Scene S(dim, 1, 1, tau);
	S.fluid.setBoundary(true, true, false, false);
	S.addBody(1, 20, { 50,50 }, { 0,0 });
	//S.fluid.setObstacle(obsX, obsY, radius);
	S.fluid.setinitCond(1.0, { 0.08, 0.0 });
	
	for (int j = 0; j < dim[1]; j++) {
		Vec2d vel;
		calcInitSpeed(0, j, vel);
		S.fluid.setvelBC(0, j, vel);
		S.fluid.setdenBC(dim[0] - 1, j, 1.0);
	}
	S.fluid.setzouBC();
<<<<<<< HEAD
	//S.setSolidFraction();
	//S.fluid.solver(10000, "LBM");
	S.solve("LBM", 10000);
	//print(S.particle.bodies[0]->functionR);
=======
	S.fluid.solver(10000, "LBM");
>>>>>>> 3aa013c6578e08f47f83e2c2d9c64f8ec3f73e4e
	
	return 0;
}
