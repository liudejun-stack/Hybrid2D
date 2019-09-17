#include "Scene.h"


void Scene::addCircle(double _mass, double _radius, Vec2d _pos, Vec2d _vel) {
	int id = eIMB.eDEM.bodies.size();
	eIMB.eDEM.bodies.push_back(std::make_shared<Body>(_mass, _radius, _pos, _vel, id));
}

void Scene::set_circlesSolid() {
	for (auto& B : eIMB.eDEM.bodies) {
		for (int j = 0; j < domainSize[1]; j++) {
			for (int i = 0; i < domainSize[0]; i++) {
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
	for (int i = 0; i < domainSize[0]; i++) {
		int id = eIMB.eLBM.getCell(i, domainSize[1] - 1);
		eIMB.eLBM.cells[id]->node = eIMB.eLBM.isSolid;
		eIMB.eLBM.cells[id]->solidFraction = 1.0;
	}
}

void Scene::set_botSolid() {
	for (int i = 0; i < domainSize[0]; i++) {
		int id = eIMB.eLBM.getCell(i, 0);
		eIMB.eLBM.cells[id]->node = eIMB.eLBM.isSolid;
		eIMB.eLBM.cells[id]->solidFraction = 1.0;
	}
}

void Scene::set_leftSolid() {
	for (int j = 0; j < domainSize[1]; j++) {
		int id = eIMB.eLBM.getCell(0, j);
		eIMB.eLBM.cells[id]->node = eIMB.eLBM.isSolid;
		eIMB.eLBM.cells[id]->solidFraction = 1.0;
	}
}

void Scene::set_rightSolid() {
	for (int j = 0; j < domainSize[1]; j++) {
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
	eIMB.eLBM.dim = domainSize;
	eIMB.eDEM.domainSize = domainSize;
	
	//Fluid parameters definition:
	eIMB.eLBM.tau = relaxationTime;
	eIMB.eLBM.kinViscosity = kinViscosity;

	//Particle parameters definition:
	eIMB.eDEM.factorOfSafety = factorOfSafety;
	eIMB.eDEM.localDamping = localDamping;
	eIMB.eDEM.frictionAngle = frictionAngle;
	eIMB.eDEM.kn = normalStiffness;
	eIMB.eDEM.ks = shearStiffness;

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

void Scene::simulationInfo(int i) {
	std::cout << "-------------------- LBM/DEM Simulation --------------------"  << "\n";
	std::cout << "Current Iteration Number: " << i                               << "\n";
	std::cout << "Solution Time Step: "       << eIMB.dt                     << "\n";
	std::cout << "Number of Bodies: "         << eIMB.eDEM.bodies.size() << "\n";
	std::cout << "Number of Cells: "          << eIMB.eLBM.cells.size()     << "\n";
}

void Scene::moveToNextTimeStep_LBM(int _nIter, std::string _fileName) {
	for (int i = 0; i != _nIter; i++) {
		eIMB.eLBM.updateMacro();
		eIMB.calculateSolidFraction();
		eIMB.calculateForceAndTorque();
		eIMB.eLBM.collision();
		eIMB.eLBM.set_bounceback();
		eIMB.eLBM.stream();
		eIMB.eDEM.demCycle();
		if (i % 100 == 0) {
			simulationInfo(i);
			eIMB.eLBM.fluidVTK(_fileName);
		}
		updateGeom();
	}
}