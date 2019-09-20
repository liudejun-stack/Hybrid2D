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

void IMB::calculateForceAndTorque() {
	for (auto& B : eDEM.bodies) {
		for (auto& C : eLBM.cells) {

			//Calculate Solid Fraction (SF):
			double inDisk = (C->cellPos - B->pos).dot((C->cellPos - B->pos));
			if (inDisk > B->radius * B->radius)	continue;
			double distCellPar = (C->cellPos - B->pos).norm();
			if (distCellPar >= (B->radius + 1.0)) {
				C->solidFraction = 0.0;
				C->node = eLBM.isFluid;
			}
			else if (distCellPar <= (B->radius - 1.0)) {
				C->solidFraction = 1.0;
				C->node = eLBM.isSolid;
			}
			else {
				double distCellParSurf = distCellPar - B->radius;
				C->solidFraction = -distCellParSurf + B->functionR;
				C->node = eLBM.fluidSolidInteraction;
			}

			//Change incorrect values of SF:
			if (C->solidFraction < 0.0)	C->solidFraction = 0.0;
			if (C->solidFraction > 1.0)	C->solidFraction = 1.0;
			ASSERT(C->solidFraction >= 0.0 && C->solidFraction <= 1.0);

			//Calculate Solid Function:
			C->solidFunction = (C->solidFraction * (eLBM.tau - 0.5)) / ((1 - C->solidFraction) + (eLBM.tau - 0.5));
			ASSERT(C->solidFunction >= 0.0 && C->solidFunction <= 1.0);

			//Calculate collision operator (Omega):
			Vec2d particleVel = B->vel;
			Vec2d Force_LBM   = Vec2d::Zero();
			for (int k = 0; k < C->Q; k++) {
				double EDF_OP = C->set_eqFun(C->rho, C->vel, C->opNode[k]);
				double EDF_Par = C->set_eqFun(C->rho, particleVel, k);

				C->omega[k] = C->f[C->opNode[k]] - EDF_OP - (C->f[k] - EDF_Par);
				Force_LBM += -C->latticeSpeed * eLBM.dx * C->solidFunction * C->omega[k] * C->discreteVelocity[k];
			}
			B->forceLBM = Force_LBM;
		}
	}
}

/*
void IMB::calculateForceAndTorque() {

	for (auto& C : eLBM.cells) {
		for (int k = 0; k < C->Q; k++) {
			eDEM.bodies[C->particleFluid_ID]->forceLBM += -C->latticeSpeed * eLBM.dx * C->omega[k] * C->discreteVelocity[k];
		}
	}
}
*/