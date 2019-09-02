/*
#include "DEM.h"


int main() {
	
	DEM D;
	
	//Parameters:
	D.gravity = { 0, -9.81 };
	D.localDamping = 0.0;
	D.frictionAngle = 30;
	D.domainSize = { 10, 10 };

	//Body addition:
	D.addBody(1, 0.5, { 5,9 }, { 0,0 });
	D.addBody(1, 0.5, { 3,9 }, { 0,0 });
	D.addBody(1, 0.5, { 7,9 }, { 0,0 });

	//Set simulation:
	D.calculateParticleTimeStep(0.5, 1, 1e6);

	//Engine
	for (int i = 0; i != 30000; i++) {
		D.demCycle();
		if (i % 100 == 0)  D.particleVTK("DEM");
	}

	return 0;
}
*/