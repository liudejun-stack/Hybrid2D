#include "IMB.h"

void IMB::defineLinkedCells() {
	for (auto& B : eDEM.bodies) {
		//Reset Interaction Vector:
		B->fluidSolidInteraction.clear();

		Vector2r cellInitPos = B->pos - (int)B->radius * Vector2r::Ones() - 0.5 * eLBM.dx * Vector2r::Ones();
		for (int j = 0; j < 2 * (int)B->radius; ++j) {
			for (int i = 0; i < 2 * (int)B->radius; ++i) {
				Vector2r aux = { i,j };
				Vector2r cellCurrentPos = cellInitPos + aux;
				if (B->fluidInteraction(cellCurrentPos, eLBM.dx)) {
					int cellID = eLBM.getCell(i, j);
					B->fluidSolidInteraction.push_back(cellID);
				}
			}
		}

		ASSERT(B->fluidSolidInteraction.size() > 0);
	}
}

double IMB::calculateSolidFraction(Vector2r& _particlePos, Vector2r& _cellPos, double _particleRadius, double _dx) {
	std::vector<Vector2r> P = { {0,0}, {0,0}, {0,0},{0,0} };
	Vector2r e1 = { 1,0 };
	Vector2r e2 = { 0,1 };
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
		Vector2r D = P[(j + 1) % 4] - P[j];
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
		B->forceLBM = Vector2r::Zero();
		for (auto& ID : B->fluidSolidInteraction) {
			auto cell = eLBM.cells[ID];
			Vector2r cellPos = cell->cellPos;
			double len = calculateSolidFraction(B->pos, cellPos, B->radius, eLBM.dx);
			if (std::abs(len) < 1.0e-12)	continue;
			double gamma = len / (4.0 * eLBM.dx);
			ASSERT(gamma >= 0.0 && gamma <= 1.0);
			cell->solidFraction = std::min(gamma + cell->solidFraction, 1.0);
			double Bn = (gamma * (eLBM.tau - 0.5)) / ((1.0 - gamma) + (eLBM.tau - 0.5));
			Vector2r velP = B->vel;
			for (int k = 0; k < cell->Q; ++k) {
				double Fvpp = cell->setEqFun(cell->rho, cell->vel, cell->opNode[k]);
				double Fvp = cell->setEqFun(cell->rho, velP, k);
				double Omega = cell->f[cell->opNode[k]] - Fvpp - (cell->f[k] - Fvp);
				cell->omega[k] += Omega;
				B->forceLBM += -Bn * Omega * cell->latticeSpeed * eLBM.dx * cell->discreteVelocity[k];
			}
		}
	}
}