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
		for (auto& C : fluid.cells) {
			double distanceCenter = std::sqrt((B->pos - C->cellPos).norm());
			if (distanceCenter >= (B->radius + fluid.dx)) {
				C->solidFraction = 0.0;
				C->node = fluid.isFluid;
			}
			else if (distanceCenter <= (B->radius - fluid.dx)) {
				C->solidFraction = 1.0;
				C->node = fluid.isSolid;
			}
			else {
				C->node = fluid.fluidSolidInteraction;
				double distanceSurface = distanceCenter - B->radius;
				C->solidFraction = -distanceSurface + B->functionR;
			}
			if (C->solidFraction < 0.0)	C->solidFraction = 0.0;
			if (C->solidFraction > 1.0)	C->solidFraction = 1.0;
			ASSERT(C->solidFraction >= 0.0 && C->solidFraction <= 1.0);
			//print(C->solidFraction);
		}
	}
}