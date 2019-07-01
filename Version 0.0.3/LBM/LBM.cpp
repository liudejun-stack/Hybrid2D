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

void LBM::updateMacro() {
	for (auto& C : cells) {
		if (C->Boundary == isFluid) {
			C->rho    =  C->f[0] + C->f[1] + C->f[2] + C->f[3] + C->f[4] + C->f[5] + C->f[6] + C->f[7] + C->f[8];
			C->vel[0] = (C->f[1] + C->f[5] + C->f[8] - C->f[3] - C->f[6] + C->f[7]) / C->rho;
			C->vel[1] = (C->f[2] + C->f[5] + C->f[6] - C->f[4] - C->f[7] + C->f[8]) / C->rho;
		}
		else {
			C->rho = 0.0;
			C->vel = Vec2d::Zero();
		}
		//std::cout << C->rho << "  " << C->vel[0] << "  " << C->vel[1] << std::endl;
	}
}

void LBM::collision() {
	for (auto& C : cells) {
		if (C->Boundary == isSolid)	continue;
		for (int k = 0; k < C->Q; k++) {
			double EDF = C->set_eqFun(C->rho, C->vel, k);
			C->f[k] = (1 - tauInv) * C->f[k] + tauInv * EDF;
			//std::cout << C->f[k] << std::endl;
		}
	}
}

void LBM::bounceback() {
	for (auto& C : cells) {
		if (C->Boundary == isFluid)	continue;
		for (int k = 0; k < C->Q; k++)	C->fTemp[k] = C->f[k];
		for (int k = 0; k < C->Q; k++)	C->f[k] = C->fTemp[C->op[k]];
	}
}

void LBM::stream() {
	for (int j = 0; j < dim[1]; j++)
	for (int i = 0; i < dim[0]; i++)
	for (int k = 0; k < cells[0]->Q; k++) {
		int idx_i = (i + cells[0]->cx[k]) % (int)dim[0];
		int idx_j = (j + cells[0]->cy[k]) % (int)dim[1];
		int idf = getCell(i, j);
		int id  = getCell(i, j);
		cells[idf]->fTemp[k] = cells[id]->f[k];
	}

	//Swap distribution function:
	for (auto& C : cells) {
		for (int k = 0; k < C->Q; k++) {
			C->f[k] = C->fTemp[k];
		}
	}
}

void LBM::outputFVTK(int _nIter, std::string _fileName) {
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
	out << "VECTORS velocity float\n";
	for (auto& C : cells) {
		out << C->vel[0] << " " << C->vel[1] << " " << 0 << "\n";
	}
	out.close();
	vtkCounter++;
}

