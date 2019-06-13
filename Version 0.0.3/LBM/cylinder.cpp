#include "Fluid.hpp"
#include <iostream>

int main() {

	double uMax = 0.1;
	double re = 100;
	Vec3i n = { 400, 100, 1 };
	int radius = n[1] / 10 + 1;
	double dx = 1.0;
	double dt = 1.0;
	double nu = uMax * (2 * radius) / re;
	int obsX = n[1] / 2;
	int obsY = n[1] / 2 + 3;

	Fluid F(nu, n, dx, dt);
	F.setGeom(true, true, false, false);
	
	F.setObstacle(obsX, obsY, radius);
    
	for (int j = 0; j < n[1]; j++) {
		double L = n[1] - 2;
		double yp = j - 1.5;
		double vx = uMax * 4 / (L*L)*(L*yp - yp * yp);
		double vy = 0.0;
		Vec3d v = { vx, vy, 0.0 };
		F.setDensBC(0,j,1.0);
		F.setVelBC(0, j, v);
		F.setDensBC(n[0]-1, j, 1.0);
	}
	
    for (int j = 0; j < n[1]; j++)
	for (int i = 0; i < n[0]; i++) {
		double rhoInit = 1.0;
<<<<<<< HEAD
		Vec3d vInit = { 0.1, 0.0, 0.0 };
=======
		Vec3d vInit = { 0.08, 0.0, 0.0 };
>>>>>>> 0a3f611cb59ffa7067adf92b5f8a8d64cd2e8eb8
		int id = F.GetCell(i, j);
		F.c[id]->setInitCond(rhoInit, vInit);
	}
	
	F.solve(10000, "resultado");

}
