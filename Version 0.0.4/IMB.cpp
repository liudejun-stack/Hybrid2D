#include "IMB.h"
#include <cmath>

void IMB::calculateTimeStep() {
	eLBM.calculateFluidTimeStep();
	eDEM.calculateParticleTimeStep();
	std::cout << eLBM.dtLBM << " " << eDEM.dtDEM << " " << eLBM.dx << "\n";
	if (eDEM.dtDEM >= eLBM.dtLBM) {
		eDEM.dtDEM = eLBM.dtLBM;
		dt = eLBM.dtLBM;
	}
	else {
		eLBM.dtLBM = eDEM.dtDEM;
		dt = eDEM.dtDEM;

		//calculate lattice spacing for new dt:
		eLBM.dx = std::sqrt(3 * eLBM.kinViscosity * eLBM.dtLBM / (eLBM.tau - 0.5));

		//Change lattice speed for all cells:
		for (auto& C : eLBM.cells) {
			C->latticeSpeed = eLBM.dx / eLBM.dtLBM;
		}
	}
	std::cout << eLBM.dtLBM << " " << eDEM.dtDEM << " " << eLBM.dx << "\n";
}

void IMB::calculateSolidFraction() {
	for (auto& B : eDEM.bodies) {
		B->forceLBM = Vec2d::Zero();
		for (int j = 0; j < eLBM.dim[1]; j++) {
			for (int i = 0; i < eLBM.dim[0]; i++) {
				int id = eLBM.getCell(i, j);
				double cir = (i - B->pos[0]) * (i - B->pos[0]) + (j - B->pos[1]) * (j - B->pos[1]);
				if (cir < (B->radius * B->radius)) {
					double distanceCenter = std::sqrt(cir);
					if (distanceCenter >= (B->radius + 1.0)) {
						eLBM.cells[id]->solidFraction = 0.0;
						eLBM.cells[id]->node = eLBM.isFluid;
					}
					else if (distanceCenter <= (B->radius - 1.0)) {
						eLBM.cells[id]->solidFraction = 1.0;
						eLBM.cells[id]->node = eLBM.isSolid;
					}
					else {
						double distanceSurface = distanceCenter - B->radius;
						eLBM.cells[id]->solidFraction = -distanceSurface + B->functionR;
						eLBM.cells[id]->node = eLBM.isFluid;
					}
					if (eLBM.cells[id]->solidFraction < 0.0)	eLBM.cells[id]->solidFraction = 0.0;
					if (eLBM.cells[id]->solidFraction > 1.0)	eLBM.cells[id]->solidFraction = 1.0;
					ASSERT(eLBM.cells[id]->solidFraction >= 0.0 && eLBM.cells[id]->solidFraction <= 1.0);

					//Calculate solid function:
					eLBM.cells[id]->solidFunction = (eLBM.cells[id]->solidFraction * (eLBM.tau - 0.5)) / ((1 - eLBM.cells[id]->solidFraction) + (eLBM.tau - 0.5));
					ASSERT(eLBM.cells[id]->solidFunction >= 0.0 && eLBM.cells[id]->solidFunction <= 1.0);
					B->Bn += eLBM.cells[id]->solidFunction;     //NÃO É ATRIBUTO DO BODY, MODIFICAR!

					//Calculate collision operator (Omega):
					Vec2d particleVel = B->vel;
					for (int k = 0; k < eLBM.cells[id]->Q; k++) {
						double EDF_OP = eLBM.cells[id]->set_eqFun(eLBM.cells[id]->rho, eLBM.cells[id]->vel, eLBM.cells[id]->opNode[k]);
						double EDF_Par = eLBM.cells[id]->set_eqFun(eLBM.cells[id]->rho, particleVel, k);

						eLBM.cells[id]->omega[k] = eLBM.cells[id]->f[eLBM.cells[id]->opNode[k]] - EDF_OP - (eLBM.cells[id]->f[k] - EDF_Par);
					}

					//Record particle ID:
					eLBM.cells[id]->particleFluid_ID = B->id;
				}
			}
		}
	}
}

void IMB::calculateForceAndTorque() {

	for (auto& C : eLBM.cells) {
		for (int k = 0; k < C->Q; k++) {
			eDEM.bodies[C->particleFluid_ID]->forceLBM += -C->latticeSpeed * eLBM.dx * eDEM.bodies[C->particleFluid_ID]->Bn * C->omega[k] * C->discreteVelocity[k];
		}
	}
}