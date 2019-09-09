#include "IMB.h"
#include <cmath>

void IMB::calculateTimeStep() {
	fluid.calculateFluidTimeStep();
	particle.calculateParticleTimeStep();
	std::cout << fluid.dtLBM << " " << particle.dtDEM << " " << fluid.dx << "\n";
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
	std::cout << fluid.dtLBM << " " << particle.dtDEM << " " << fluid.dx << "\n";
}

void IMB::calculateSolidFraction() {
	for (auto& B : particle.bodies) {
		ASSERT(particle.bodies.size() > 0);
		for (int j = 0; j < fluid.dim[1]; j++) {
			for (int i = 0; i < fluid.dim[0]; i++) {
				int id = fluid.getCell(i, j);
				double cir = (i - B->pos[0]) * (i - B->pos[0]) + (j - B->pos[1]) * (j - B->pos[1]);
				if (cir < (B->radius * B->radius)) {
					double distanceCenter = std::sqrt(cir);
					if (distanceCenter >= (B->radius + 1.0)) {
						fluid.cells[id]->solidFraction = 0.0;
						fluid.cells[id]->node = fluid.isFluid;
					}
					else if (distanceCenter <= (B->radius - 1.0)) {
						fluid.cells[id]->solidFraction = 1.0;
						fluid.cells[id]->node = fluid.isSolid;
					}
					else {
						double distanceSurface = distanceCenter - B->radius;
						fluid.cells[id]->solidFraction = -distanceSurface + B->functionR;
						fluid.cells[id]->node = fluid.fluidSolidInteraction;
					}
					if (fluid.cells[id]->solidFraction < 0.0)	fluid.cells[id]->solidFraction = 0.0;
					if (fluid.cells[id]->solidFraction > 1.0)	fluid.cells[id]->solidFraction = 1.0;
					ASSERT(fluid.cells[id]->solidFraction >= 0.0 && fluid.cells[id]->solidFraction <= 1.0);

					//Calculate solid function:
					fluid.cells[id]->solidFunction = (fluid.cells[id]->solidFraction * (fluid.tau - 0.5)) / ((1 - fluid.cells[id]->solidFraction) + (fluid.tau - 0.5));
					ASSERT(fluid.cells[id]->solidFunction >= 0.0 && fluid.cells[id]->solidFunction <= 1.0);

					//Calculate collision operator (Omega):
					for (int k = 0; k < fluid.cells[id]->Q; k++) {
						double EDF_OP = fluid.cells[id]->set_eqFun(fluid.cells[id]->rho, fluid.cells[id]->vel, fluid.cells[id]->opNode[k]);
						double EDF_Par = fluid.cells[id]->set_eqFun(fluid.cells[id]->rho, B->vel, k);

						fluid.cells[id]->omega[k] = fluid.cells[id]->f[fluid.cells[id]->opNode[k]] - EDF_OP - (fluid.cells[id]->f[k] - EDF_Par);
					}
				}
			}
		}
	}
}

void IMB::calculateForceAndTorque() {
	
}