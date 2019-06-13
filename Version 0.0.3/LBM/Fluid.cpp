#include "Fluid.hpp"

int Fluid::GetCell(int i, int j) {
	return i + nDim[0] * j;
}

void Fluid::setGeom(bool _top, bool _bot, bool _left, bool _right) {
	if (_top) {
		for (int i = 0; i < nDim[0]; i++) {
			int id = GetCell(i, nDim[1]-1);
			c[id]->Boundary = isSolid;
		}
	}

	if (_bot) {
		for (int i = 0; i < nDim[0]; i++) {
			int id = GetCell(i, 0);
			c[id]->Boundary = isSolid;
		}
	}

	if (_left) {
		for (int j = 0; j < nDim[1]; j++) {
			int id = GetCell(0, j);
			c[id]->Boundary = isSolid;
		}
	}

	if (_right) {
		for (int j = 0; j < nDim[1]; j++) {
			int id = GetCell(nDim[0]-1, j);
			c[id]->Boundary = isSolid;
		}
	}
}

void Fluid::setObstacle(int _obsX, int _obsY, int _radius) {

    for (int j = 0; j < nDim[1]; j++)
    for (int i = 0; i < nDim[0]; i++){
        int cir = (i-_obsX)*(i-_obsX) + (j - _obsY)*(j - _obsY);
        if (cir < (_radius*_radius)) {
			int id = GetCell(i, j);
			c[id]->Boundary = isSolid;
        }
    }
}

void Fluid::setDensBC(int i, int j, double _rho) {
	int id = GetCell(i, j);
	c[id]->rho = _rho;
}

void Fluid::setVelBC(int i, int j, Vec3d& _vel) {
	int id = GetCell(i, j);
	c[id]->vel[0] = _vel[0];
	c[id]->vel[1] = _vel[1];
}
/*
void Fluid::macroUpdate(){
	for (int i = 0; i < Ncells; i++){

		if(c[i]->Boundary==isFluid){
			c[i]->rho    = c[i]->f[0] + c[i]->f[1] + c[i]->f[2] + c[i]->f[3] + c[i]->f[4] + c[i]->f[5] + c[i]->f[6] + c[i]->f[7] + c[i]->f[8];
			c[i]->vel[0] = (c[i]->f[5] + c[i]->f[1] + c[i]->f[8] - c[i]->f[6] - c[i]->f[2] - c[i]->f[7])/c[i]->rho;
			c[i]->vel[1] = (c[i]->f[5] + c[i]->f[2] + c[i]->f[6] - c[i]->f[7] - c[i]->f[4] - c[i]->f[8])/c[i]->rho;
		}
		else{
			c[i]->rho    = 0.0;
			c[i]->vel[0] = 0.0;
			c[i]->vel[1] = 0.0;
		}
	}
}
*/
void Fluid::Collision() {
	for (int i = 0; i < Ncells; i++) {
		if (c[i]->Boundary==isSolid)	continue;
		for (int k = 0; k < c[0]->Q; k++) {
			Vec3d  v;
            double rho = c[i]->macroUpdate(v);
			double EDF = c[i]->setEqFun(rho, v, k);
			c[i]->f[k] = (1 - omega)*c[i]->f[k] + omega * EDF;
		}
	}
}

void Fluid::BounceBack() {
	for (int i = 0; i < Ncells; i++) {
		if (c[i]->Boundary==isFluid)	continue;
		for (int k = 0; k < c[0]->Q; k++)	c[i]->fTemp[k] = c[i]->f[k];
		for (int k = 0; k < c[0]->Q; k++)	c[i]->f[k] = c[i]->fTemp[c[i]->op[k]];
	}
}

void Fluid::Stream() {
	for (int i = 0; i < Ncells; i++) {
		//if (c[id]->Boundary==isSolid)	continue;
		for (int k = 0; k < c[0]->Q; k++) {
			c[c[i]->nQ[k]]->fTemp[k] = c[i]->f[k];
		}
	}

	for (int i = 0; i < Ncells; i++) {
		for (int k = 0; k < c[0]->Q; k++){
            c[i]->f[k] = c[i]->fTemp[k];
        }
	}
	
	for (int i = 0; i < Ncells; i++){
        c[i]->rho = c[i]->macroUpdate(c[i]->vel);
    }
}

void Fluid::writeFVTK(std::string _filename){

	std::ofstream Output;
	Output.open(_filename + std::to_string(vtkCounter) + ".vtk");

	Output << "# vtk DataFile Version 3.0\n";
	Output << "Fluid_state\n";
	Output << "ASCII\n";
	Output << "DATASET STRUCTURED_POINTS\n";
	Output << "DIMENSIONS " << nDim[0] << " " << nDim[1] << " " << 1 << "\n";
	Output << "ORIGIN " << 0 << " " << 0 << " " << 0 << "\n";
	Output << "SPACING " << 1 << " " << 1 << " " << 1 << "\n";
	Output << "POINT_DATA " << Ncells << "\n";

	Output << "SCALARS Geom float 1\n";
	Output << "LOOKUP_TABLE default\n";
	for (int i = 0; i < Ncells; i++) {
		Output << c[i]->Boundary << "\n";
	}

	Output << "SCALARS Density float 1\n";
	Output << "LOOKUP_TABLE default\n";
	for (int i = 0; i < Ncells; i++) {
		Output << c[i]->rho << "\n";
	}

	Output << "VECTORS Velocity float\n" << std::endl;
	for (int i = 0; i < Ncells; i++) {
		Vec3d v = c[i]->vel;
		Output << v[0] << " " << v[1] << " " << 0 << "\n";
	}
	Output.close();
	vtkCounter++;
}

void Fluid::solve(int nIter, std::string _filename) {
	for (int i = 0; i != nIter; i++) {
		std::cout << i << std::endl;
		Collision();
		BounceBack();
		Stream();
		if (i % 100 == 0)   writeFVTK(_filename);
	}
}
