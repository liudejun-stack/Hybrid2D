#include "IMB.h"

void IMB::defineLinkedCells() {
	for (auto& B : eDEM.bodies) {
		//Reset Interaction Vector:
		B->fluidSolidInteraction.clear();

		Vec2d cellInitPos = B->pos - 2 * (int)B->radius * Vec2d::Ones() - 0.5 * eLBM.dx * Vec2d::Ones();
		for (int j = 0; j < 4 * (int)B->radius; ++j) {
			for (int i = 0; i < 4 * (int)B->radius; ++i) {
				Vec2d aux = { i,j };
				Vec2d cellCurrentPos = cellInitPos + aux;
				if (B->fluidInteraction(cellCurrentPos, eLBM.dx)) {
					int cellID = eLBM.getCell(i, j);
					B->fluidSolidInteraction.push_back(cellID);
				}
			}
		}

		ASSERT(B->fluidSolidInteraction.size() > 0);
	}
}

double IMB::calculateSolidFraction(Vec2d& _particlePos, Vec2d& _cellPos, double _particleRadius, double _dx) {
	std::vector<Vec2d> P = { {0,0}, {0,0}, {0,0},{0,0} };
	Vec2d e1 = { 1,0 };
	Vec2d e2 = { 0,1 };
	P[0] = _cellPos - 0.5 * _dx * e1 - 0.5 * _dx * e2;
	P[1] = _cellPos + 0.5 * _dx * e1 - 0.5 * _dx * e2;
	P[2] = _cellPos + 0.5 * _dx * e1 + 0.5 * _dx * e2;
	P[3] = _cellPos - 0.5 * _dx * e1 + 0.5 * _dx * e1;

	double dmin = 2 * _particleRadius;
	double dmax = 0.0;

	ASSERT(P.size() == 4);
	for (int j = 0; j < P.size(); ++j) {
		double dist = (P[j] - _particlePos).norm();
		if (dmin > dist)	dmin = dist;
		if (dmax < dist)	dmax = dist;
	}

	if (dmin > _particleRadius + _dx)	return 0.0;

	if (dmax < _particleRadius) {
		return 4.0 * _dx;
	}

	double len = 0.0;
	for (int j = 0; j < 4; ++j) {
		Vec2d D = P[(j + 1) % 4] - P[j];
		double a = D.dot(D);
		double b = 2 * (P[j] - _particlePos).dot(D);
		double c = (P[j] - _particlePos).dot(P[j] - _particlePos) - _particleRadius * _particleRadius;

		if (b * b - 4 * a * c > 0.0) {
			double ta = (-b - std::sqrt(b * b - 4 * a * c)) / (2 * a);
			double tb = (-b + std::sqrt(b * b - 4 * a * c)) / (2 * a);
			if (ta > 1.0 && tb > 1.0)	continue;
			if (ta < 0.0 && tb < 0.0)	continue;
			if (ta < 0.0)	ta = 0.0;
			if (tb > 1.0)	tb = 1.0;

			len += ((tb - ta) * D).norm();
		}
	}
	return len;
}

void IMB::calculateForceAndTorque() {
	for (auto& B : eDEM.bodies) {
		B->forceLBM = Vec2d::Zero();
		for (auto& ID : B->fluidSolidInteraction) {
			Vec2d cellPos = eLBM.cells[ID]->cellPos;
			double len = calculateSolidFraction(B->pos, cellPos, B->radius, eLBM.dx);
			if (std::abs(len) < 1.0e-12)	continue;
			double gamma = len / (4.0 * eLBM.dx);
			ASSERT(gamma >= 0.0 && gamma <= 1.0);
			eLBM.cells[ID]->solidFraction = std::min(gamma + eLBM.cells[ID]->solidFraction, 1.0);
			//std::cout << eLBM.cells[ID]->solidFraction << std::endl;
			double Bn = (gamma * (eLBM.tau - 0.5)) / ((1.0 - gamma) + (eLBM.tau - 0.5));
			Vec2d velP = B->vel;
			for (int k = 0; k < eLBM.cells[ID]->Q; ++k) {
				double Fvpp = eLBM.cells[ID]->setEqFun(eLBM.cells[ID]->rho, eLBM.cells[ID]->vel, eLBM.cells[ID]->opNode[k]);
				double Fvp = eLBM.cells[ID]->setEqFun(eLBM.cells[ID]->rho, velP, k);
				double Omega = eLBM.cells[ID]->f[eLBM.cells[ID]->opNode[k]] - Fvpp - (eLBM.cells[ID]->f[k] - Fvp);
				eLBM.cells[ID]->omega[k] += Omega;
				B->forceLBM += -Bn * Omega * eLBM.cells[ID]->latticeSpeed * eLBM.dx * eLBM.cells[ID]->discreteVelocity[k];
			}
		}
	}
}