#include "Lattice.h"

double Lattice::set_eqFun(double _rho, Vec2d _vel, int k) {
	double cdotv = discreteVelocity[k].dot(_vel);
	double vdotv = _vel.dot(_vel);
	return weight[k] * _rho * (1 + 3 * cdotv / (latticeSpeed) + 4.5 * cdotv * cdotv / (latticeSpeed * latticeSpeed) - 1.5 * vdotv / (latticeSpeed * latticeSpeed));
}
