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
		if (C->node == eIMB.eLBM.fluidSolidInteraction) {
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
	eIMB.eLBM.tau = relaxationTime;
	eIMB.eLBM.kinViscosity = kinViscosity;

	//Particle parameters definition:
	eIMB.eDEM.factorOfSafety = factorOfSafety;
	eIMB.eDEM.localDamping = localDamping;
	eIMB.eDEM.frictionAngle = frictionAngle;
	eIMB.eDEM.normalStiffness = normalStiffness;
	eIMB.eDEM.shearStiffness = shearStiffness;

	//Cell initialization for LBM simualtion:
	eIMB.eLBM.initializeCells();

	//Setting solids for LBM simulation:
	if (top_isSolid)	set_topSolid();
	if (bot_isSolid)	set_botSolid();
	if (right_isSolid)	set_rightSolid();
	if (left_isSolid)	set_rightSolid();
	if (bodies_isSolid)	set_circlesSolid();

	//Time step calculation:
	eIMB.calculateTimeStep();
	//eIMB.eDEM.calculateParticleTimeStep();
}

void Scene::simulationInfo(int& i) {
	if (i == 0) {
		std::cout << "----------------------- LBM/DEM Simulation -----------------------"  << "\n";
		std::cout << "Current Iteration Number: " << i                                     << "\n";
		std::cout << "Domain Size:              " << domainSize[0] << "x" << domainSize[1] << "\n";
		std::cout << "Number of Bodies:         " << eIMB.eDEM.bodies.size()               << "\n";
		std::cout << "Number of cells:          " << eIMB.eLBM.cells.size()                << "\n";
		std::cout << "Solution TimeStep:        " << eIMB.dt                               << "\n";
		std::cout << "Lattice Spacing:          " << eIMB.eLBM.dx                          << "\n";
		std::cout << "Relaxation Time:          " << relaxationTime                        << "\n";
		std::cout << "Kinematic Viscosity:      " << kinViscosity                          << "\n";
		std::cout << "Friction Angle:           " << frictionAngle                         << "\n";
		std::cout << "Normal Stiffness:         " << normalStiffness                       << "\n";
		std::cout << "Shear Stiffness:          " << shearStiffness                        << "\n";
		std::cout << "Local Damping:            " << localDamping                          << "\n";
		std::cout << "Particle Kinematic Energy: " << eIMB.eDEM.kinEnergy.back()          << "\n";
		std::cout << "Particle Potential Energy: " << eIMB.eDEM.potEnergy.back()          << "\n";
		std::cout << "Total Energy: " << eIMB.eDEM.kinEnergy.back() + eIMB.eDEM.potEnergy.back() << "\n";
	}
	else {
		std::cout << "----------------------- LBM/DEM Simulation -----------------------" << "\n";
		std::cout << "Current Iteration Number:   " << i                                  << "\n";
		std::cout << "Particle Kinematic Energy : " << eIMB.eDEM.kinEnergy.back() << "\n";
		std::cout << "Particle Potential Energy : " << eIMB.eDEM.potEnergy.back() << "\n";
		std::cout << "Total Energy: " << eIMB.eDEM.kinEnergy.back() + eIMB.eDEM.potEnergy.back() << "\n";
	}
}

void Scene::moveToNextTimeStep_LBM(int _nIter, std::string _fileName) {
	for (int i = 0; i != _nIter; ++i) {
		eIMB.eDEM.forceResetter();
		//eIMB.eLBM.updateMacro();
		//eIMB.calculateSolidFraction();
		//eIMB.eLBM.collision();
		//eIMB.eLBM.set_bounceback();
		//eIMB.eLBM.stream();
		eIMB.eDEM.contactVerification();
		eIMB.eDEM.forceCalculation();
		eIMB.eDEM.updateVelPos();
		eIMB.eDEM.updateContact();
		if (i % 100 == 0)	eIMB.eLBM.fluidVTK(_fileName);
		if (i % 1000 == 0) {
			eIMB.eDEM.calculateEnergy();
			simulationInfo(i);
		}
		updateGeom();
	}
}

void Scene::moveToNextTimeStep_DEM(int _nIter, std::string _fileName) {
	for (int i = 0; i != _nIter; ++i) {
		eIMB.eDEM.forceResetter();
		eIMB.eDEM.contactVerification();
		eIMB.eDEM.forceCalculation();
		eIMB.eDEM.updateVelPos();
		eIMB.eDEM.updateContact();
		if (i % 100 == 0)	eIMB.eDEM.particleVTK(_fileName);
	}
}