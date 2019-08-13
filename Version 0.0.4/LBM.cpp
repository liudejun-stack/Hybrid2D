#include "LBM.h"

int LBM::getCell(int i, int j) {
	return i + dim[0] * j;
}

void LBM::setBoundary(bool _top, bool _bot, bool _left, bool _right) {
	if (_top) {
		for (int i = 0; i < dim[0]; i++) {
			int id = getCell(i, dim[1] - 1);
			cells[id]->Boundary = isSolid;
		}
	}

	if (_bot) {
		for (int i = 0; i < dim[0]; i++) {
			int id = getCell(i, 0);
			cells[id]->Boundary = isSolid;
		}
	}

	if (_left) {
		for (int j = 0; j < dim[1]; j++) {
			int id = getCell(0, j);
			cells[id]->Boundary = isSolid;
		}
	}

	if (_right) {
		for (int j = 0; j < dim[1]; j++) {
			int id = getCell(dim[0] - 1, j);
			cells[id]->Boundary = isSolid;
		}
	}
}

void LBM::setObstacle(int _obsX, int _obsY, int _radius) {
	for (int j = 0; j < dim[1]; j++)
	for (int i = 0; i < dim[0]; i++) {
		int cir = (i - _obsX) * (i - _obsX) + (j - _obsY) * (j - _obsY);
		if (cir < _radius * _radius) {
			int id = getCell(i, j);
			cells[id]->Boundary = isSolid;
		}
	}
}

void LBM::setinitCond(double _rhoInit, Vec2d _vel) {
	for (auto& C : cells) {
		if (C->Boundary == isSolid)	continue;
		for (int k = 0; k < C->Q; k++) {
			C->f[k] = C->set_eqFun(_rhoInit, _vel, k);
			//std::cout << C->f[k] << std::endl;
		}
	}
}

void LBM::setvelBC(int i, int j, Vec2d _vel) {
	int id = getCell(i, j);
	cells[id]->vel = _vel;
}

void LBM::setdenBC(int i, int j, double _rho) {
	int id = getCell(i, j);
	cells[id]->rho = _rho;
}

void LBM::setzouBC() {
	double div = 1.0 / 6.0;
	double aux = 2.0 / 3.0;
	for (auto& C : cells) {
		if (C->Boundary == isSolid)	continue;
		//Prescribed velocity (Left & Right sides)
		for (int j = 0; j < dim[1]; j++) {
			if (C->ID == getCell(0, j)) {
				//Left side:
				double rho = (C->f[0] + C->f[2] + C->f[4] + 2.0 * (C->f[3] + C->f[6] + C->f[7])) / (1.0 - C->vel[0]);

				C->f[1] = C->f[3] + aux * rho * C->vel[0];
				C->f[5] = C->f[7] + div * rho * C->vel[0] + 0.5 * rho * C->vel[1] - 0.5 * (C->f[2] - C->f[4]);
				C->f[8] = C->f[6] + div * rho * C->vel[0] - 0.5 * rho * C->vel[1] + 0.5 * (C->f[2] - C->f[4]);
			}
			/*
			if (C->ID == getCell(dim[0] - 1, j)) {
				//Right side:
				double rho = (C->f[0] + C->f[2] + C->f[4] + 2.0 * (C->f[1] + C->f[5] + C->f[8])) / (1.0 + C->vel[0]);

				C->f[3] = C->f[1] - aux * rho * C->vel[0];
				C->f[7] = C->f[5] - div * rho * C->vel[0] + 0.5 * (C->f[2] - C->f[4]);
				C->f[6] = C->f[8] - div * rho * C->vel[0] - 0.5 * (C->f[2] - C->f[4]);
			}
			*/
		}
		//Prescribed density
		for (int j = 0; j < dim[1]; j++) {
			if (C->ID == getCell(dim[0] - 1, j)) {
				double vx = -1.0 + (C->f[0] + C->f[2] + C->f[4] + 2.0 * (C->f[1] + C->f[5] + C->f[8])) / C->rho;

				C->f[3] = C->f[1] - aux * C->rho * vx;
				C->f[7] = C->f[5] - div * C->rho * vx + 0.5 * (C->f[2] + C->f[4]);
				C->f[6] = C->f[8] - div * C->rho * vx - 0.5 * (C->f[2] + C->f[4]);
			}
		}
	}
}

void LBM::updateMacro() {
	for (auto& C : cells) {
		if (C->Boundary == isFluid) {
			C->rho    =  C->f[0] + C->f[1] + C->f[2] + C->f[3] + C->f[4] + C->f[5] + C->f[6] + C->f[7] + C->f[8];
			C->vel[0] = latticeSpeed * ((C->f[1] + C->f[5] + C->f[8] - C->f[3] - C->f[6] - C->f[7]) / C->rho);
			C->vel[1] = latticeSpeed * ((C->f[2] + C->f[5] + C->f[6] - C->f[4] - C->f[7] - C->f[8]) / C->rho);
		}
		else {
			C->rho = 0.0;
			C->vel = Vec2d::Zero();
		}
	}
}

void LBM::collision() {
	ASSERT(tau > 0.5);
	for (auto& C : cells) {
		if (C->Boundary == isSolid)	continue;
		for (int k = 0; k < C->Q; k++) {
			double EDF = C->set_eqFun(C->rho, C->vel, k);
			//C->f[k] = C->f[k] - (1 - C->solidFunction) * tauInv * (C->f[k] - EDF) + C->solidFunction * C->omega[k];
			C->f[k] = (1 - tauInv) * C->f[k] + tauInv * EDF;
			//std::cout << C->f[k] << std::endl;
		}
	}
}

void LBM::bounceback() {
	for (auto& C : cells) {
		if (C->Boundary == isFluid)	continue;
		for (int k = 0; k < C->Q; k++)	C->fTemp[k] = C->f[k];
		for (int k = 0; k < C->Q; k++)	C->f[k] = C->fTemp[C->opNode[k]];
	}
}

void LBM::stream() {
	for (int i = 0; i < Ncells; i++) 
	for (int k = 0; k < cells[0]->Q; k++) {
		cells[cells[i]->nCell[k]]->fTemp[k] = cells[i]->f[k];
	}
	//Swap distribution function:
	for (auto& C : cells) {
		for (int k = 0; k < C->Q; k++) {
			C->f[k] = C->fTemp[k];
			//std::cout << C->f[k] << std::endl;
		}
	}
}

void LBM::outputFVTK(std::string _fileName) {
	std::ofstream out;
	out.open(_fileName + std::to_string(vtkCounter) + ".vtk");
	out << "# vtk DataFile Version 3.0\n";
	out << "Fluid state\n";
	out << "ASCII\n";
	out << "DATASET STRUCTURED_POINTS\n";
	out << "DIMENSIONS " << dim[0] << " " << dim[1] << " " << 1 << "\n";
	out << "ORIGIN "     << 0      << " " << 0      << " " << 0 << "\n";
	out << "SPACING "    << 1      << " " << 1      << " " << 1 << "\n";
	out << "POINT_DATA " << dim[0] * dim[1] << "\n";
	out << "SCALARS Geometry float 1\n";
	out << "LOOKUP_TABLE default\n";
	for (auto& C : cells) {
		out << C->Boundary << "\n";
	}
	out << "SCALARS Density float 1\n";
	out << "LOOKUP_TABLE default\n";
	for (auto& C : cells) {
		out << C->rho << "\n";
	}
	out << "VECTORS Velocity float\n";
	for (auto& C : cells) {
		out << C->vel[0] << " " << C->vel[1] << " " << 0 << "\n";
	}
	out.close();
	vtkCounter++;
}

void LBM::solver(int _nIter, std::string _fileName) {
	for (int i = 0; i != _nIter; i++) {
		print(i);
		updateMacro();
		collision();
		bounceback();
		stream();
		if (i % 100 == 0)	outputFVTK(_fileName);
	}
}

