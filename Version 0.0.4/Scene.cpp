#include "Scene.h"


void Scene::addCircle(double _mass, double _radius, Vec2d _pos, Vec2d _vel) {
	int id = eIMB.eDEM.bodies.size();
	eIMB.eDEM.bodies.push_back(std::make_shared<Body>(_mass, _radius, _pos, _vel, id));
}

void Scene::set_circlesSolid() {
	for (auto& B : eIMB.eDEM.bodies) {
		for (int j = 0; j < domainSize[1]; ++j) {
			for (int i = 0; i < domainSize[0]; ++i) {
				int id = eIMB.eLBM.getCell(i, j);
				double cir = (i - B->pos[0]) * (i - B->pos[0]) + (j - B->pos[1]) * (j - B->pos[1]);
				if (cir < B->radius * B->radius) {
					eIMB.eLBM.cells[id]->node = eIMB.eLBM.isSolid;
				}
			}
		}
	}
}

void Scene::set_topSolid() {
	for (int i = 0; i < domainSize[0]; ++i) {
		int id = eIMB.eLBM.getCell(i, domainSize[1] - 1);
		eIMB.eLBM.cells[id]->node = eIMB.eLBM.isSolid;
		eIMB.eLBM.cells[id]->solidFraction = 1.0;
	}
}

void Scene::set_botSolid() {
	for (int i = 0; i < domainSize[0]; ++i) {
		int id = eIMB.eLBM.getCell(i, 0);
		eIMB.eLBM.cells[id]->node = eIMB.eLBM.isSolid;
		eIMB.eLBM.cells[id]->solidFraction = 1.0;
	}
}

void Scene::set_leftSolid() {
	for (int j = 0; j < domainSize[1]; ++j) {
		int id = eIMB.eLBM.getCell(0, j);
		eIMB.eLBM.cells[id]->node = eIMB.eLBM.isSolid;
		eIMB.eLBM.cells[id]->solidFraction = 1.0;
	}
}

void Scene::set_rightSolid() {
	for (int j = 0; j < domainSize[1]; ++j) {
		int id = eIMB.eLBM.getCell(domainSize[0] - 1, j);
		eIMB.eLBM.cells[id]->node = eIMB.eLBM.isSolid;
		eIMB.eLBM.cells[id]->solidFraction = 1.0;
	}
}

void Scene::updateGeom() {
	for (auto& C : eIMB.eLBM.cells) {
		if (C->node == eIMB.eLBM.isSolid || C->node == eIMB.eLBM.fluidSolidInteraction) {
			C->node = eIMB.eLBM.isFluid;
		}
	}
	set_topSolid();
	set_botSolid();
	set_circlesSolid();
}

void Scene::prepareScenario() {

	//Boundary definition:
	eIMB.eLBM.domainSize = domainSize;
	eIMB.eDEM.domainSize = domainSize;

	//Fluid parameters definition:
	eIMB.eLBM.dx           = latticeSpacing;
	eIMB.eLBM.tau          = relaxationTime;
	eIMB.eLBM.kinViscosity = kinViscosity;

	//Particle parameters definition:
	eIMB.eDEM.factorOfSafety  = factorOfSafety;
	eIMB.eDEM.localDamping    = localDamping;
	eIMB.eDEM.frictionAngle   = frictionAngle;
	eIMB.eDEM.normalStiffness = normalStiffness;
	eIMB.eDEM.shearStiffness  = shearStiffness;

	//Time step calculation:
	eIMB.calculateTimeStep();
	//eIMB.eDEM.calculateParticleTimeStep();

	//Cell initialization for LBM simualtion:
	eIMB.eLBM.initializeCells();

	//Setting solids for LBM simulation:
	if (top_isSolid)	set_topSolid();
	if (bot_isSolid)	set_botSolid();
	if (right_isSolid)	set_rightSolid();
	if (left_isSolid)	set_rightSolid();
	if (bodies_isSolid)	set_circlesSolid();

	//Directories for VTK output
	int ignore;
	ignore = system("mkdir -p fluidVTK");
	ignore = system("mkdir -p solidVTK");
}

void Scene::simulationInfo(int& i) {
	double totalEnergy = eIMB.eDEM.kinEnergy.back() + eIMB.eDEM.potEnergy.back();
	if (i == 0) {
		std::cout << "----------------------- LBM/DEM Simulation -----------------------"  << "\n";
		std::cout << "Current Iteration Number: " << i                                     << "\n";
		std::cout << "             Domain Size: " << domainSize[0] << "x" << domainSize[1] << "\n";
		std::cout << "        Number of Bodies: " << eIMB.eDEM.bodies.size()               << "\n";
		std::cout << "         Number of cells: " << eIMB.eLBM.cells.size()                << "\n";
		std::cout << "       Solution TimeStep: " << eIMB.dt                               << "\n";
		std::cout << "         Lattice Spacing: " << eIMB.eLBM.dx                          << "\n";
		std::cout << "         Relaxation Time: " << relaxationTime                        << "\n";
		std::cout << "     Kinematic Viscosity: " << kinViscosity                          << "\n";
		std::cout << "          Friction Angle: " << frictionAngle                         << "\n";
		std::cout << "        Normal Stiffness: " << normalStiffness                       << "\n";
		std::cout << "         Shear Stiffness: " << shearStiffness                        << "\n";
		std::cout << "           Local Damping: " << localDamping                          << "\n";
		std::cout << "  (DEM) Kinematic Energy: " << eIMB.eDEM.kinEnergy.back()            << "\n";
		std::cout << "  (DEM) Potential Energy: " << eIMB.eDEM.potEnergy.back()            << "\n";
		std::cout << "  (DEM)     Total Energy: " << totalEnergy                           << "\n";
	}
	else {
		std::cout << "----------------------- LBM/DEM Simulation -----------------------" << "\n";
		std::cout << "Current Iteration Number: " << i                                    << "\n";
		std::cout << "  (DEM) Kinematic Energy: " << eIMB.eDEM.kinEnergy.back()           << "\n";
		std::cout << "  (DEM) Potential Energy: " << eIMB.eDEM.potEnergy.back()           << "\n";
		std::cout << "  (DEM)     Total Energy: " << totalEnergy                          << "\n";
	}
}

void Scene::fluidVTK(std::string _fileName) {
	std::ofstream out;
	out.open("fluidVTK/" + _fileName + std::to_string(vtkCounter) + ".vtk");
	out << "# vtk DataFile Version 3.0\n";
	out << "Fluid state\n";
	out << "ASCII\n";
	out << "DATASET STRUCTURED_POINTS\n";
	out << "DIMENSIONS " << domainSize[0] << " " << domainSize[1] << " " << 1 << "\n";
	out << "ORIGIN " << 0 << " " << 0 << " " << 0 << "\n";
	out << "SPACING " << 1 << " " << 1 << " " << 1 << "\n";
	out << "POINT_DATA " << domainSize[0] * domainSize[1] << "\n";
	out << "SCALARS Geometry float 1\n";
	out << "LOOKUP_TABLE default\n";
	for (auto& C : eIMB.eLBM.cells) {
		out << C->node << "\n";
	}
	out << "SCALARS Density float 1\n";
	out << "LOOKUP_TABLE default\n";
	for (auto& C : eIMB.eLBM.cells) {
		out << C->rho << "\n";
	}
	out << "VECTORS Velocity float\n";
	for (auto& C : eIMB.eLBM.cells) {
		out << C->vel[0] << " " << C->vel[1] << " " << 0 << "\n";
	}
	out.close();
	vtkCounter++;
}

void Scene::solidVTK(std::string _fileName) {
	std::ofstream out;
	out.open("solidVTK/" + _fileName + std::to_string(vtkCounter) + ".vtk");
	out << "# vtk DataFile Version 3.0\n";
	out << "DEM\n";
	out << "ASCII\n";
	out << " \n";
	out << "DATASET POLYDATA\n";
	out << "POINTS " << std::to_string((int)eIMB.eDEM.bodies.size()) << " float\n";
	for (auto& B : eIMB.eDEM.bodies) {
		out << B->pos[0] << " " << B->pos[1] << " " << 0 << std::endl;
	}
	out << "POINT_DATA " << std::to_string((int)eIMB.eDEM.bodies.size()) << std::endl;
	out << "SCALARS radii double\n";
	out << "LOOKUP_TABLE default\n";
	for (auto& B : eIMB.eDEM.bodies) {
		out << B->radius << std::endl;
	}
	out.close();
	vtkCounter++;
}

void Scene::moveToNextTimeStep_LBM() {
		eIMB.eDEM.forceResetter();
		eIMB.eLBM.updateMacro();
		eIMB.calculateSolidFraction();
		eIMB.eLBM.collision();
		eIMB.eLBM.set_bounceback();
		eIMB.eLBM.stream();
}

void Scene::moveToNextTimeStep_DEM() {
		eIMB.eDEM.forceResetter();
		eIMB.eDEM.contactVerification();
		eIMB.eDEM.forceCalculation();
		eIMB.eDEM.updateVelPos();
		eIMB.eDEM.updateContact();
}

void Scene::moveToNextTimeStep(int _nIter, std::string _fileName) {

}