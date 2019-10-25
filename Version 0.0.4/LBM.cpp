#include "LBM.h"

int LBM::getCell(int i, int j) {return i + domainSize[0] * j;}

void LBM::setVelBC(int i, int j, Vec2d _vel) {
	int id = getCell(i, j);
	cells[id]->vel = _vel;
}

void LBM::setDenBC(int i, int j, double _rho) {
	int id = getCell(i, j);
	cells[id]->rho = _rho;
}

void LBM::setZouBC() {
	double div = 1.0 / 6.0;
	double aux = 2.0 / 3.0;
	for (auto& C : cells) {
		if (C->node == isSolid)	continue;
		//Prescribed velocity (Left & Right sides)
		for (int j = 0; j < domainSize[1]; ++j) {

			if (C->ID == getCell(0, j)) {
				//Left side:
				double rho = (C->f[0] + C->f[2] + C->f[4] + 2.0 * (C->f[3] + C->f[6] + C->f[7])) / (1.0 - C->vel[0]);

				C->f[1] = C->f[3] + aux * rho * C->vel[0];
				C->f[5] = C->f[7] + div * rho * C->vel[0] + 0.5 * rho * C->vel[1] - 0.5 * (C->f[2] - C->f[4]);
				C->f[8] = C->f[6] + div * rho * C->vel[0] - 0.5 * rho * C->vel[1] + 0.5 * (C->f[2] - C->f[4]);
			}
			/*
			if (C->ID == getCell(domainSize[0] - 1, j)) {
				//Right side:
				double rho = (C->f[0] + C->f[2] + C->f[4] + 2.0 * (C->f[1] + C->f[5] + C->f[8])) / (1.0 + C->vel[0]);

				C->f[3] = C->f[1] - aux * rho * C->vel[0];
				C->f[7] = C->f[5] - div * rho * C->vel[0] + 0.5 * (C->f[2] - C->f[4]);
				C->f[6] = C->f[8] - div * rho * C->vel[0] - 0.5 * (C->f[2] - C->f[4]);
			}

			if (C->ID == getCell(j, domainSize[1] - 1)) {
				double rho = (C->f[0] + C->f[1] + C->f[3] + 2.0 * (C->f[2] + C->f[5] + C->f[6])) / (1.0 - C->vel[1]);

				C->f[4] = C->f[2] - aux * rho * C->vel[1];
				C->f[7] = C->f[5] - div * rho * C->vel[1] + 0.5 * (C->f[1] - C->f[3]);
				C->f[8] = C->f[6] - div * rho * C->vel[1] + 0.5 * (C->f[3] - C->f[1]);
			}
			*/
		}
		//Prescribed density
		for (int j = 0; j < domainSize[1]; ++j) {

			if (C->ID == getCell(domainSize[0] - 1, j)) {
				double vx = -1.0 + (C->f[0] + C->f[2] + C->f[4] + 2.0 * (C->f[1] + C->f[5] + C->f[8])) / C->rho;

				C->f[3] = C->f[1] - aux * C->rho * vx;
				C->f[7] = C->f[5] - div * C->rho * vx + 0.5 * (C->f[2] + C->f[4]);
				C->f[6] = C->f[8] - div * C->rho * vx - 0.5 * (C->f[2] + C->f[4]);
				/*
				if (C->ID == getCell(j, 0)) {
					double vx = -1.0 + (C->f[0] + C->f[1] + C->f[3] + 2.0 * (C->f[4] + C->f[7] + C->f[8])) / C->rho;

					C->f[2] = C->f[4] - aux * C->rho * vx;
					C->f[5] = C->f[7] - div * C->rho * vx + 0.5 * (C->f[3] - C->f[1]);
					C->f[6] = C->f[8] - div * C->rho * vx + 0.5 * (C->f[1] - C->f[3]);
				}
				*/
			}
		}
	}
}

void LBM::initializeCells() {
	ASSERT(dtLBM > 0.0);
	for (int j = 0; j < domainSize[1]; ++j)
		for (int i = 0; i < domainSize[0]; ++i) {
			Vec2d cellPos = { i,j };
			int id = cells.size();
			cells.emplace_back(std::make_shared<Lattice>(id, dx, dtLBM, tau, domainSize, cellPos));
		}
}

void LBM::setInitCond(double _rhoInit, Vec2d _vel) {
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
		for (int k = 0; k < C->Q; ++k) {
			C->omega[k] = 0.0;
		}
	}
}

void LBM::applyForce() {
	for (auto& C : cells) {
		C->sourceForce = C->rho * gravity;
	}
}

void LBM::collision() {
	ASSERT(tau > 0.5);
	double tauInv = 1.0 / tau;
	for (auto& C : cells) {
		if (C->node == isSolid)	continue;
		Vec2d Vel = C->vel + C->sourceForce * dtLBM / (2 * C->rho);
		double solidFunction = (C->solidFraction * (tau - 0.5)) / ((1 - C->solidFraction) + (tau - 0.5));
		for (int k = 0; k < C->Q; k++) {
			double EDF = C->setEqFun(C->rho, Vel, k);
			double source = C->setSourceTerm(tau, dtLBM, k);
			C->f[k] = (1 - tauInv) * C->f[k] + tauInv * EDF;
			//C->f[k] = C->f[k] - (1 - solidFunction) * tauInv * (C->f[k] - EDF) + solidFunction * C->omega[k];
		}
	}
}

void LBM::setBounceBack() {
	for (auto& C : cells) {
		if (C->node == isFluid)	continue;
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
