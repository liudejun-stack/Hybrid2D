#include "IMB.h"
#include <cmath>

void IMB::calculateTimeStep() {
	if (particle.dtDEM >= fluid.dtLBM) {
		particle.dtDEM = fluid.dtLBM;
		dt = fluid.dtLBM;
	}
	else {
		fluid.dtLBM = particle.dtDEM;
		dt = particle.dtDEM;

		//calculate lattice spacing for new dt:
		fluid.dx = std::sqrt(3 * fluid.kinViscosity * fluid.dtLBM / (fluid.tau - 0.5));

		//Change lattice speed for all cells:
		for (auto& C : fluid.cells) {
			C->latticeSpeed = fluid.dx / fluid.dtLBM;
		}
	}
}

void IMB::calculateSolidFraction() {
	for (auto& B : particle.bodies) {
		ASSERT(particle.bodies.size() > 0);
		for (int j = 0; j < fluid.dim[1]; j++) {
			for (int i = 0; i < fluid.dim[0]; i++) {
				double cir = (i - B->pos[0]) * (i - B->pos[0]) + (j - B->pos[1]) * (j - B->pos[1]);
				if (cir < (B->radius * B->radius)){
					int id = fluid.getCell(i, j);
					fluid.cells[id]->node = fluid.isSolid;
				}
			}
		}
	}
}
/*
for (auto& C : fluid.cells) {
	double distanceCenter = std::sqrt((C->cellPos - B->pos).norm());
	if (distanceCenter <= B->radius) {
		/*
		if (distanceCenter >= B->radius + fluid.dx) {
			C->solidFraction = 1.0;
			C->node = fluid.isSolid;
		}
		else if (distanceCenter <= B->radius - fluid.dx) {
			C->solidFraction = 0.0;
			C->node = fluid.isFluid;
		}
		else {
			double distanceSurface = distanceCenter - B->radius;
			C->solidFraction = -distanceSurface + B->functionR;
			C->node = fluid.fluidSolidInteraction;
		}
		
	}
}
*/