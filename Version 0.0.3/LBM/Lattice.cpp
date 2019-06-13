#include "Lattice.hpp"

void Lattice::cellInfo(){
	std::cout << "Relaxation time: " << tau << std::endl;
	std::cout << "Lattice speed: " << cs << std::endl;
	if(Boundary){
		std::cout << "Solid boundary";
	}
	else{
		std::cout << "Fluid boundary";
	}
}

double Lattice::macroUpdate(Vec3d& _vel)
{
    if(Boundary)    return 0.0;
    double rho = 0.0;
    _vel = {0.0, 0.0, 0.0};
    for (int k = 0; k < Q; k++){
        _vel[0] += f[k]*cx[k];
        _vel[1] += f[k];
        rho += f[k];
    }
    _vel[0] = _vel[0]/rho;
    _vel[1] = _vel[1]/rho;
    return rho;
}

double Lattice::setEqFun(double _rho, Vec3d& _vel, int k) {
	double vdotc = _vel[0] * cx[k] + _vel[1] * cy[k];
	double vdotv = _vel[0] * _vel[0] + _vel[1] * _vel[1];
	return w[k] * _rho*(1.0 + 3.0*vdotc + 4.5*vdotc*vdotc - 1.5*vdotv);
}

void Lattice::setInitCond(double _rho, Vec3d& _vel) {
	for (int k = 0; k < Q; k++)	f[k] = setEqFun(_rho, _vel, k);
}
