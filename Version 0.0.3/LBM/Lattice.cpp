#include "Lattice.h"

double Lattice::set_eqFun(int k) {
	double cdotv = cx[k] * vel[0] + cy[k] * vel[1];
	double vdotv = vel.dot(vel);
	return w[k] * rho * (1 + 3 * cdotv + 4.5 * cdotv * cdotv - 1.5 * vdotv);
}
