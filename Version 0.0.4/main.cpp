
#include "Scene.h"


int main() {
	
	Scene S;

	S.addCircle(1, 5, { 250, 50 }, { 0,0 });
	S.addCircle(1, 5, { 300, 50 }, { 0,0 });
	S.coupling.fluid.dim = { 500, 100 };
	S.coupling.particle.domainSize = { 500, 100 };
	S.domainSize = { 500, 100 };
	S.coupling.fluid.initializeCells();
	S.set_topSolid();
	S.set_botSolid();
	S.coupling.calculateSolidFraction();
	//S.set_circlesSolid();
	S.coupling.calculateTimeStep();
	S.coupling.fluid.fluidVTK("LBM");

	return 0;

}
