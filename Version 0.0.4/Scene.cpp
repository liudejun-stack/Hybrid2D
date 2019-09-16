#include "Scene.h"


void Scene::addCircle(double _mass, double _radius, Vec2d _pos, Vec2d _vel) {
	int id = coupling.particle.bodies.size();
	coupling.particle.bodies.push_back(std::make_shared<Body>(_mass, _radius, _pos, _vel, id));
}

void Scene::set_circlesSolid() {
	for (auto& B : coupling.particle.bodies) {
		for (int j = 0; j < domainSize[1]; j++) {
			for (int i = 0; i < domainSize[0]; i++) {
				int id = coupling.fluid.getCell(i, j);
				double cir = (i - B->pos[0]) * (i - B->pos[0]) + (j - B->pos[1]) * (j - B->pos[1]);
				if (cir < B->radius * B->radius) {
					coupling.fluid.cells[id]->node = coupling.fluid.isSolid;
				}
			}
		}
	}
}

void Scene::set_topSolid() {
	for (int i = 0; i < domainSize[0]; i++) {
		int id = coupling.fluid.getCell(i, domainSize[1] - 1);
		coupling.fluid.cells[id]->node = coupling.fluid.isSolid;
		coupling.fluid.cells[id]->solidFraction = 1.0;
	}
}

void Scene::set_botSolid() {
	for (int i = 0; i < domainSize[0]; i++) {
		int id = coupling.fluid.getCell(i, 0);
		coupling.fluid.cells[id]->node = coupling.fluid.isSolid;
		coupling.fluid.cells[id]->solidFraction = 1.0;
	}
}

void Scene::set_leftSolid() {
	for (int j = 0; j < domainSize[1]; j++) {
		int id = coupling.fluid.getCell(0, j);
		coupling.fluid.cells[id]->node = coupling.fluid.isSolid;
		coupling.fluid.cells[id]->solidFraction = 1.0;
	}
}

void Scene::set_rightSolid() {
	for (int j = 0; j < domainSize[1]; j++) {
		int id = coupling.fluid.getCell(domainSize[0] - 1, j);
		coupling.fluid.cells[id]->node = coupling.fluid.isSolid;
		coupling.fluid.cells[id]->solidFraction = 1.0;
	}
}

void Scene::updateGeom() {
	for (auto& C : coupling.fluid.cells) {
		if (C->node == coupling.fluid.fluidSolidInteraction) {
			C->node = coupling.fluid.isFluid;
		}
	}
	set_topSolid();
	set_botSolid();
	set_circlesSolid();
}

void Scene::prepareScenario() {
	//Boundary definition:
	coupling.fluid.dim = domainSize;
	coupling.particle.domainSize = domainSize;
	
	//Fluid parameters definition:
	coupling.fluid.tau = relaxationTime;
	coupling.fluid.kinViscosity = kinViscosity;

	//Particle parameters definition:
	coupling.particle.factorOfSafety = factorOfSafety;
	coupling.particle.localDamping = localDamping;
	coupling.particle.frictionAngle = frictionAngle;
	coupling.particle.kn = normalStiffness;
	coupling.particle.ks = shearStiffness;

	//Cell initialization for LBM simualtion:
	coupling.fluid.initializeCells();

	//Setting solids for LBM simulation:
	if (top_isSolid)	set_topSolid();
	if (bot_isSolid)	set_botSolid();
	if (right_isSolid)	set_rightSolid();
	if (left_isSolid)	set_rightSolid();
	if (bodies_isSolid)	set_circlesSolid();

	//Time step calculation:
	coupling.calculateTimeStep();
	//coupling.particle.calculateParticleTimeStep();
}

void Scene::simulationInfo(int i) {
	std::cout << "-------------------- LBM/DEM Simulation --------------------"  << "\n";
	std::cout << "Current Iteration Number: " << i                               << "\n";
	std::cout << "Solution Time Step: "       << coupling.dt                     << "\n";
	std::cout << "Number of Bodies: "         << coupling.particle.bodies.size() << "\n";
	std::cout << "Number of Cells: "          << coupling.fluid.cells.size()     << "\n";
}

void Scene::moveToNextTimeStep_LBM(int _nIter, std::string _fileName) {
	for (int i = 0; i != _nIter; i++) {
		coupling.fluid.updateMacro();
		coupling.calculateSolidFraction();
		coupling.calculateForceAndTorque();
		coupling.fluid.collision();
		coupling.fluid.set_bounceback();
		coupling.fluid.stream();
		//coupling.particle.demCycle();
		if (i % 100 == 0) {
			simulationInfo(i);
			coupling.fluid.fluidVTK(_fileName);
		}
		//updateGeom();
	}
}