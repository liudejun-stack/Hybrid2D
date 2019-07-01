#include "Lattice.h"

double Lattice::set_eqFun(double _rho, Vec2d _vel, int k) {
	double cdotv = cx[k] * _vel[0] + cy[k] * _vel[1];
	double vdotv = _vel.dot(_vel);
	return w[k] * _rho * (1 + 3 * cdotv + 4.5 * cdotv * cdotv - 1.5 * vdotv);
}
