#include "LBM.h"

int LBM::getCell(int i, int j) { return i + domainSize[0] * j; }

void LBM::setVelBC(int i, int j, Vector2r _vel) {
	int id = getCell(i, j);
	cells[id]->velBC = _vel;
}

void LBM::setDenBC(int i, int j, double _rho) {
	int id = getCell(i, j);
	cells[id]->rhoBC = _rho;
}

void LBM::setZouBC() {
	double div = 1.0 / 6.0;
	double aux = 2.0 / 3.0;


	for (auto& C : cells) {
		if (C->node == C->isSolid)	continue;

		//Velocity Zou&He Conditions (Sukop & Thorne)
		//North Side
		if (setVelNorth) {
			for (int i = 0; i < domainSize[0]; ++i) {
				if (C->ID == getCell(i, domainSize[1] - 1)) {
					double rho = (C->f[0] + C->f[1] + C->f[3] + 2.0 * (C->f[2] + C->f[5] + C->f[6])) / (1.0 + C->velBC[1]);
					C->f[4] = C->f[2] - aux * rho * C->velBC[1];
					C->f[7] = C->f[5] - div * rho * C->velBC[1] + 0.5 * (C->f[1] - C->f[3]);
					C->f[8] = C->f[6] - div * rho * C->velBC[1] + 0.5 * (C->f[3] - C->f[1]);
					C->updateMacro();
				}
			}
		}
		//South Side
		if (setVelSouth) {
			for (int i = 0; i < domainSize[0]; ++i) {
				if (C->ID == getCell(i, 0)) {
					double rho = (C->f[0] + C->f[1] + C->f[3] + 2.0 * (C->f[4] + C->f[7] + C->f[8])) / (1.0 - C->velBC[1]);
					C->f[2] = C->f[4] + aux * rho * C->velBC[1];
					C->f[5] = C->f[7] + div * rho * C->velBC[1] - 0.5 * (C->f[1] - C->f[3]);
					C->f[6] = C->f[8] + div * rho * C->velBC[1] - 0.5 * (C->f[3] - C->f[1]);
					C->updateMacro();
				}
			}
		}
		//East side
		if (setVelEast) {
			for (int j = 0; j < domainSize[1]; ++j) {
				if (C->ID == getCell(domainSize[0] - 1, j)) {
					double rho = (C->f[0] + C->f[2] + C->f[4] + 2.0 * (C->f[1] + C->f[5] + C->f[8])) / (1.0 + C->velBC[0]);
					C->f[3] = C->f[1] - aux * rho * C->velBC[0];
					C->f[7] = C->f[5] - div * rho * C->velBC[0] + 0.5 * (C->f[2] - C->f[4]);
					C->f[6] = C->f[8] - div * rho * C->velBC[0] + 0.5 * (C->f[4] - C->f[2]);
					C->updateMacro();
				}
			}
		}
		//West side
		if (setVelWest) {
			for (int j = 0; j < domainSize[1]; ++j) {
				if (C->ID == getCell(0, j)) {
					if (C->node == C->isSolid)	continue;
					double rho = (C->f[0] + C->f[2] + C->f[4] + 2.0 * (C->f[3] + C->f[6] + C->f[7])) / (1.0 - C->velBC[0]);
					C->f[1] = C->f[3] + aux * rho * C->velBC[0];
					C->f[5] = C->f[7] + div * rho * C->velBC[0] - 0.5 * (C->f[2] - C->f[4]);
					C->f[8] = C->f[6] + div * rho * C->velBC[0] - 0.5 * (C->f[4] - C->f[2]);
					C->updateMacro();
				}
			}
		}

		//Prescribed density
		//North Side
		if (setDenNorth) {
			for (int i = 0; i < domainSize[0]; ++i) {
				if (C->ID == getCell(i, domainSize[1] - 1)) {
					double vy = -1.0 + (C->f[0] + C->f[1] + C->f[3] + 2.0 * (C->f[2] + C->f[5] + C->f[6])) / C->rhoBC;

					C->f[4] = C->f[2] - aux * C->rhoBC * vy;
					C->f[7] = C->f[5] - div * C->rhoBC * vy + 0.5 * (C->f[1] - C->f[3]);
					C->f[8] = C->f[6] - div * C->rhoBC * vy + 0.5 * (C->f[3] - C->f[1]);
					C->updateMacro();
				}
			}
		}
		//South Side
		if (setDenSouth) {
			for (int i = 0; i < domainSize[0]; ++i) {
				if (C->ID == getCell(i, 0)) {
					double vy = -1.0 + (C->f[0] + C->f[1] + C->f[3] + 2.0 * (C->f[4] + C->f[7] + C->f[8])) / C->rhoBC;

					C->f[2] = C->f[4] - aux * C->rhoBC * vy;
					C->f[5] = C->f[7] - div * C->rhoBC * vy + 0.5 * (C->f[3] - C->f[1]);
					C->f[6] = C->f[8] - div * C->rhoBC * vy + 0.5 * (C->f[1] - C->f[3]);
					C->updateMacro();
				}
			}
		}
		//East Side
		if (setDenEast) {
			for (int j = 0; j < domainSize[1]; ++j) {
				if (C->ID == getCell(domainSize[0] - 1, j)) {
					if (C->node == C->isSolid)	continue;
					double vx = -1.0 + (C->f[0] + C->f[2] + C->f[4] + 2.0 * (C->f[1] + C->f[5] + C->f[8])) / C->rhoBC;
					C->f[3] = C->f[1] - aux * C->rhoBC * vx;
					C->f[7] = C->f[5] - div * C->rhoBC * vx + 0.5 * (C->f[2] - C->f[4]);
					C->f[6] = C->f[8] - div * C->rhoBC * vx + 0.5 * (C->f[4] - C->f[2]);
					C->updateMacro();
				}
			}
		}
		//West Side
		if (setDenWest) {
			for (int j = 0; j < domainSize[1]; ++j) {
				if (C->ID == getCell(0, j)) {
					double vx = -1.0 + (C->f[0] + C->f[2] + C->f[4] + 2.0 * (C->f[3] + C->f[6] + C->f[7])) / C->rhoBC;
					C->f[1] = C->f[3] + aux * C->rhoBC * vx;
					C->f[5] = C->f[7] + div * C->rhoBC * vx + 0.5 * (C->f[4] - C->f[2]);
					C->f[8] = C->f[6] + div * C->rhoBC * vx + 0.5 * (C->f[2] - C->f[4]);
					C->updateMacro();
				}
			}
		}
	}
}

void LBM::initializeCells() {
	ASSERT(dtLBM > 0.0);
	for (int j = 0; j < domainSize[1]; ++j)
		for (int i = 0; i < domainSize[0]; ++i) {
			Vector2r cellPos = { i,j };
			int id = getCell(i, j);
			cells.push_back(std::make_shared<Lattice>(id, dx, dtLBM, tau, domainSize, cellPos));
		}
}

void LBM::setInitCond(double _rhoInit, Vector2r _vel) {
	for (auto& C : cells) {
		for (int k = 0; k < C->Q; k++) {
			C->f[k] = C->setEqFun(_rhoInit, _vel, k);
		}
		C->updateMacro();
	}
}

void LBM::resetSolidFraction() {
	for (auto& C : cells) {
		C->previousSolidFraction = C->solidFraction;
		C->solidFraction = 0.0;
		C->node = 0;
		for (int k = 0; k < C->Q; ++k) {
			C->omega[k] = 0.0;
		}

		/*if (C->previousSolidFraction == C->isSolid) {
			C->solidFraction = 1.0;
			C->node = 1;
		}*/
	}
}

void LBM::applyBodyForce() {
	for (auto& C : cells) {
		C->sourceForce = C->rho * gravity;
	}
}

void LBM::collision() {
	ASSERT(tau > 0.5);
	double tauInv = 1.0 / tau;
	for (auto& C : cells) {
		if (C->node == C->isSolid)	continue;
		Vector2r Vel;
		double density = C->getDensityAndVelocity(Vel);
		for (int k = 0; k < C->Q; k++) {
			double EDF = C->setEqFun(density, Vel, k);
			C->f[k] = (1 - tauInv) * C->f[k] + tauInv * EDF;
		}
	}
}

void LBM::collisionNT() {
	ASSERT(tau > 0.5);
	double tauInv = 1.0 / tau;
	for (auto& C : cells) {
		if (C->node == C->isSolid)	continue;
		Vector2r Vel;
		double density = C->getDensityAndVelocity(Vel);
		double solidFunction = (C->solidFraction * (tau - 0.5)) / ((1.0 - C->solidFraction) + (tau - 0.5));
		for (int k = 0; k < C->Q; ++k) {
			double EDF = C->setEqFun(density, Vel, k);
			C->f[k] = C->f[k] - (1 - solidFunction) * tauInv * (C->f[k] - EDF) + solidFunction * C->omega[k];
		}
	}
}

void LBM::setBounceBack() {
	for (auto& C : cells) {
		if (C->node == C->isFluid)	continue;
		for (int k = 0; k < C->Q; k++)	C->fTemp[k] = C->f[k];
		for (int k = 0; k < C->Q; k++)	C->f[k] = C->fTemp[C->opNode[k]];
	}
}

void LBM::stream() {
	double Ncells = domainSize[0] * domainSize[1];
	for (int i = 0; i < Ncells; ++i)
		for (int k = 0; k < cells[0]->Q; k++) {
			cells[cells[i]->nCell[k]]->fTemp[k] = cells[i]->f[k];
		}

	//Swap distribution function:
	for (auto& C : cells) {
		std::vector<double> Temp = C->f;
		C->f = C->fTemp;
		C->fTemp = Temp;
		C->updateMacro();
	}
}