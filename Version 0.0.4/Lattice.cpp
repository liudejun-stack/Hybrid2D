#include "Lattice.h"

double Lattice::set_eqFun(double _rho, Vec2d _vel, int k) {
	double cdotv = discreteVelocity[k].dot(_vel);
	double vdotv = _vel.dot(_vel);
	return weight[k] * _rho * (1 + 3 * cdotv / (latticeSpeed) + 4.5 * cdotv * cdotv / (latticeSpeed * latticeSpeed) - 1.5 * vdotv / (latticeSpeed * latticeSpeed));
}

double Lattice::set_sourceTerm(double& _tau, double _dt, int k) {
	Vec2d c = discreteVelocity[k] * latticeSpeed;
	Vec2d cminusv = c - vel;
	double cdotv = discreteVelocity[k].dot(vel);
	return (1 - _dt / (2 * _tau)) * weight[k] * (3 / (latticeSpeed * latticeSpeed) * cminusv + 9 / (latticeSpeed * latticeSpeed) * cdotv * discreteVelocity[k]).dot(sourceForce);
}
