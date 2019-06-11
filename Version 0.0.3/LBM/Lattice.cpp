#include "Lattice.hpp"

double Lattice::Density() {
	if (isSolid)	return 0.0;
	double rho = 0.0;
	for (int k = 0; k < Q; k++)	rho += f[k];
	return rho;
}

void Lattice::Velocity(Vec3d& _vel) {
	if (isSolid) {
		for (int i = 0; i < _vel.size(); i++)	_vel[i] = 0.0;
	}
	double rho = Density();
	for (int k = 0; k < Q; k++) {
		_vel[0] += f[k] * cx[k];
		_vel[1] += f[k] * cy[k];
		_vel[2] = 0.0;
	}
	for (int i = 0; i < _vel.size(); i++)	_vel[i] = _vel[i] / rho;
}

double Lattice::setEqFun(double _rho, Vec3d& _vel, int k) {
	double vdotc = _vel[0] * cx[k] + _vel[1] * cy[k];
	double vdotv = _vel[0] * _vel[0] + _vel[1] * _vel[1];
	return w[k] * _rho*(1.0 + 3.0*vdotc + 4.5*vdotc*vdotc - 1.5*vdotv);
}

void Lattice::setInitCond(double _rho, Vec3d& _vel) {
	for (int k = 0; k < Q; k++)	f[k] = setEqFun(_rho, _vel, k);
}
