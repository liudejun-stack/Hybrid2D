#include "IMB.h"

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
		for (auto& C : eLBM.cells) {
			double distCellPar = (C->cellPos - B->pos).dot((C->cellPos - B->pos));
			if (distCellPar <= B->radius * B->radius) {

				double len = calculateSolidFraction(B->pos, C->cellPos, B->radius, eLBM.dx);
				if (std::abs(len) < 1.0e-12)	continue;
				double gamma = len / (4.0 * eLBM.dx);
				ASSERT(gamma >= 0.0 && gamma <= 1.0);
				C->solidFraction = std::min(gamma + C->solidFraction, 1.0);
				double Bn = (gamma * (eLBM.tau - 0.5)) / ((1.0 - gamma) + (eLBM.tau - 0.5));
				Vec2d velP = B->vel;
				for (int k = 0; k < C->Q; ++k) {
					double Fvpp = C->setEqFun(C->rho, C->vel, C->opNode[k]);
					double Fvp = C->setEqFun(C->rho, velP, k);
					double Omega = C->f[C->opNode[k]] - Fvpp - (C->f[k] - Fvp);

					C->omega[k] += Omega;
					B->forceLBM += -Bn * Omega * C->latticeSpeed * eLBM.dx * C->discreteVelocity[k];
				}
			}
		}
	}
}

