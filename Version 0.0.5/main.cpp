/*
#include "Scene.h"


int main() {
	
	Scene S;

	S.addCircle(1, 1, { 3,9 }, { 0,0 });
	S.addCircle(1, 1, { 5,9 }, { 0,0 });
	S.set_boundary({ 10, 10 });
	S.coupling.calculateTimeStep();
	S.coupling.particle.frictionAngle = 30;
	S.coupling.particle.localDamping = 0;

	for (int i = 0; i < 30000; i++) {
		S.coupling.particle.demCycle();
		if (i % 100 == 0)	S.coupling.particle.particleVTK("DEM");
	}

	return 0;

}
*/