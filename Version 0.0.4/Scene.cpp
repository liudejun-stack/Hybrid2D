#include "Scene.h"

void Scene::addCircle(double _mass, double _radius, Vec2d _pos, Vec2d _vel) {
	int id = eIMB.eDEM.bodies.size();
	eIMB.eDEM.bodies.push_back(std::make_shared<Body>(_mass, _radius, _pos, _vel, id));
}

void Scene::setDomain() {

	//Set Bodies solid
	if (bodiesAreSolid) {
		for (auto& B : eIMB.eDEM.bodies) {
			for (auto& C : eIMB.eLBM.cells) {
				bool inContact = B->fluidInteraction(C->cellPos, C->dx);
				if (inContact) {
					C->node = C->isSolid;
					C->solidFraction = 1.0;
				}
			}
		}
	}

	//Set Top Solid
	if (topIsSolid) {
		for (int i = 0; i < domainSize[0]; ++i) {
			int id = eIMB.eLBM.getCell(i, domainSize[1] - 1);
			eIMB.eLBM.cells[id]->node = eIMB.eLBM.cells[id]->isSolid;
			eIMB.eLBM.cells[id]->solidFraction = 1.0;
		}
	}

	//Set Bot Solid
	if (botIsSolid) {
		for (int i = 0; i < domainSize[0]; ++i) {
			int id = eIMB.eLBM.getCell(i, 0);
			eIMB.eLBM.cells[id]->node = eIMB.eLBM.cells[id]->isSolid;
			eIMB.eLBM.cells[id]->solidFraction = 1.0;
		}
	}

	//Set Left Solid
	if (leftIsSolid) {
		for (int j = 0; j < domainSize[1]; ++j) {
			int id = eIMB.eLBM.getCell(0, j);
			eIMB.eLBM.cells[id]->node = eIMB.eLBM.cells[id]->isSolid;
			eIMB.eLBM.cells[id]->solidFraction = 1.0;
		}
	}

	if (rightIsSolid) {
		for (int j = 0; j < domainSize[1]; ++j) {
			int id = eIMB.eLBM.getCell(domainSize[0] - 1, j);
			eIMB.eLBM.cells[id]->node = eIMB.eLBM.cells[id]->isSolid;
			eIMB.eLBM.cells[id]->solidFraction = 1.0;
		}
	}
}

void Scene::prepareScenario() {

	//Boundary definition:
	eIMB.eLBM.domainSize      = domainSize;
	eIMB.eDEM.domainSize      = domainSize;

	//Fluid parameters definition:
	eIMB.eLBM.dx              = latticeSpacing;
	eIMB.eLBM.tau             = relaxationTime;
	eIMB.eLBM.kinViscosity    = kinViscosity;

	//Particle parameters definition:
	eIMB.eDEM.factorOfSafety  = factorOfSafety;
	eIMB.eDEM.localDamping    = localDamping;
	eIMB.eDEM.frictionAngle   = frictionAngle;
	eIMB.eDEM.normalStiffness = normalStiffness;
	eIMB.eDEM.shearStiffness  = shearStiffness;

	//Calculate DEM TimeStep
	eIMB.eDEM.calculateParticleTimeStep();
	subCycleNumber  = (int)(eIMB.eLBM.dtLBM / eIMB.eDEM.dtDEM) + 1;
	eIMB.eDEM.dtDEM = (eIMB.eLBM.dtLBM / subCycleNumber);

	//Cell initialization for LBM simualtion:
	eIMB.eLBM.initializeCells();

	//Setting solids for LBM simulation:
	setDomain();
}

void Scene::simulationInfo(int& i) {
	std::system("cls");
	double totalEnergy = eIMB.eDEM.kinEnergy.back() + eIMB.eDEM.potEnergy.back();

	std::cout << "----------------------- LBM/DEM Simulation --------------------------------" << "\n";
	std::cout << "   Iteration Number: " << i                                                  << "\n";
	std::cout << "        Domain Size: " << domainSize[0] << "x" << domainSize[1]              << "\n";
	std::cout << "   Number of Bodies: " << eIMB.eDEM.bodies.size()                            << "\n";
	std::cout << "    Number of cells: " << eIMB.eLBM.cells.size()                             << "\n";
	std::cout << "    Simulation Time: " << std::setprecision(3) << Time << "/" << simDuration << "\n";

	std::cout << "----------------------- LBM Parameters ------------------------------------" << "\n";
	std::cout << "          Time Step: " << eIMB.eLBM.dtLBM                                    << "\n";
	std::cout << "    Lattice Spacing: " << eIMB.eLBM.dx                                       << "\n";
	std::cout << "    Relaxation Time: " << relaxationTime                                     << "\n";
	std::cout << "Kinematic Viscosity: " << kinViscosity                                       << "\n";

	std::cout << "----------------------- DEM Parameters ------------------------------------" << "\n";
	std::cout << "          Time Step: " << eIMB.eDEM.dtDEM                                    << "\n";
	std::cout << "    SubCycle Number: " << subCycleNumber                                     << "\n";
	std::cout << "     Friction Angle: " << frictionAngle                                      << "\n";
	std::cout << "   Normal Stiffness: " << normalStiffness                                    << "\n";
	std::cout << "    Shear Stiffness: " << shearStiffness                                     << "\n";
	std::cout << "      Local Damping: " << localDamping                                       << "\n";
	std::cout << "   Kinematic Energy: " << eIMB.eDEM.kinEnergy.back()                         << "\n";
	std::cout << "   Potential Energy: " << eIMB.eDEM.potEnergy.back()                         << "\n";
	std::cout << "       Total Energy: " << totalEnergy                                        << "\n";
}

void Scene::fluidVTK(std::string _fileName) {
	std::ofstream out;
	out.open("VTK_Fluid/" + _fileName + std::to_string(fluidVtkCounter) + ".vtk");
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
	fluidVtkCounter++;
}

void Scene::solidVTK(std::string _fileName) {
	std::ofstream out;
	out.open("VTK_Solid/" + _fileName + std::to_string(particleVtkCounter) + ".vtk");
	out << "# vtk DataFile Version 3.0\n";
	out << "DEM\n";
	out << "ASCII\n";
	out << "DATASET POLYDATA\n";
	out << "POINTS " << std::to_string((int)eIMB.eDEM.bodies.size()) << " float\n";
	for (auto& B : eIMB.eDEM.bodies) {
		out << B->pos[0] << " " << B->pos[1] << " " << 0 << "\n";
	}
	out << "POINT_DATA " << std::to_string((int)eIMB.eDEM.bodies.size()) << std::endl;
	out << "SCALARS radii float\n";
	out << "LOOKUP_TABLE default\n";
	for (auto& B : eIMB.eDEM.bodies) {
		out << B->radius << std::endl;
	}
	out.close();
	particleVtkCounter++;
}

void Scene::LBMSolver() {

	//Create Directory
	int ignore = system("mkdir VTK_Fluid");
	int i = 0;

	while (Time < simDuration) {

		//Print Simulation Info
		if (i % 1000 == 0) {
			eIMB.eDEM.calculateEnergy();
			simulationInfo(i);
		}

		//Fluid Engine
		//eIMB.eLBM.setZouBC();
		eIMB.eLBM.collision();
		eIMB.eLBM.setBounceBack();
		eIMB.eLBM.stream();

		//Output VTK
		if (i % 100 == 0)	fluidVTK("LBM");
		Time += eIMB.eLBM.dtLBM;
		++i;
	}
}

void Scene::DEMSolver() {
	//Create Directory
	int ignore = system("mkdir VTK_Solid");
	
	for (int i = 0; i != simDuration; ++i) {

		//Print Simulation Info
		if (i % 1000 == 0) {
			eIMB.eDEM.calculateEnergy();
			simulationInfo(i);
		}

		//Solid Engine
		eIMB.eDEM.contactVerification();
		eIMB.eDEM.forceCalculation();
		eIMB.eDEM.updateVelPos();
		eIMB.eDEM.updateContact();

		//Output VTK
		if (i % 100 == 0)	solidVTK("DEM");
	}
}

void Scene::moveToNextTimeStep() {

	//Create directories
	int ignore = system("mkdir VTK_Fluid");
	    ignore = system("mkdir VTK_Solid");

	double tlbm = 0.0;
	int i = 0;
	while (Time < simDuration) {
		eIMB.eLBM.resetSolidFraction();
		eIMB.defineLinkedCells();
		eIMB.calculateForceAndTorque();
		eIMB.eDEM.contactVerification();
		eIMB.eDEM.forceCalculation();
		eIMB.eDEM.updateVelPos();
		eIMB.eDEM.updateContact();
		if (Time >= tlbm) {
			eIMB.eLBM.collision();
			eIMB.eLBM.setBounceBack();
			eIMB.eLBM.stream();
			tlbm += eIMB.eLBM.dtLBM;
		}
		Time += eIMB.eDEM.dtDEM;
		eIMB.updateFluidSolidContact();

		if (i % 1000 == 0) {
			eIMB.eDEM.calculateEnergy();
			simulationInfo(i);
			fluidVTK("LBM");
			solidVTK("DEM");
		}
		++i;
	}
}