#include "Lattice.h"

double Lattice::setEqFun(double _rho, Vector2r _vel, int k) {
	double cdotv = discreteVelocity[k].dot(_vel);
	double vdotv = _vel.dot(_vel);
	return weight[k] * _rho * (1 + (3 * cdotv / latticeSpeed) + (4.5 * cdotv * cdotv / (latticeSpeed * latticeSpeed)) - (1.5 * vdotv / (latticeSpeed * latticeSpeed)));
}

double Lattice::setSourceTerm(double& _tau, double _dt, int k) {
	Vector2r c = discreteVelocity[k] * latticeSpeed;
	Vector2r cminusv = c - vel;
	double cdotv = discreteVelocity[k].dot(vel);
	return (1 - _dt / (2 * _tau)) * weight[k] * (3 / (latticeSpeed * latticeSpeed) * cminusv + 9 / (latticeSpeed * latticeSpeed) * cdotv * discreteVelocity[k]).dot(sourceForce);
}

double Lattice::getDensityAndVelocity(Vector2r& _vel) {
	double density;
	if (node == isSolid) {
		density = 0.0;
		_vel    = Vector2r::Zero();
		return density;
	}
	density = f[0] + f[1] + f[2] + f[3] + f[4] + f[5] + f[6] + f[7] + f[8];
	_vel[0] = latticeSpeed * ((f[1] + f[5] + f[8] - f[3] - f[6] - f[7]) / density);
	_vel[1] = latticeSpeed * ((f[2] + f[5] + f[6] - f[4] - f[7] - f[8]) / density);

	ASSERT(!(std::isnan(density) || std::isnan(_vel.norm())));
	return density;
}

void Lattice::updateMacro() {
	if (node == isSolid) {
		vel = Vector2r::Zero();
		rho = 0.0;
	}
	else {
		rho    = f[0] + f[1] + f[2] + f[3] + f[4] + f[5] + f[6] + f[7] + f[8];
		vel[0] = latticeSpeed * ((f[1] + f[5] + f[8] - f[3] - f[6] - f[7]) / rho);
		vel[1] = latticeSpeed * ((f[2] + f[5] + f[6] - f[4] - f[7] - f[8]) / rho);
	}
	ASSERT(!(std::isnan(rho) || std::isnan(vel.norm())));
}
