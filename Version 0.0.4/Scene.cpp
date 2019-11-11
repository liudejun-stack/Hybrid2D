#include "Scene.h"

static Scene scene;
Scene& Scene::getScene() { return scene; }

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
	//subCycleNumber  = (int)(eIMB.eLBM.dtLBM / eIMB.eDEM.dtDEM) + 1;
	//eIMB.eDEM.dtDEM = (eIMB.eLBM.dtLBM / subCycleNumber);

	//Cell initialization for LBM simualtion:
	eIMB.eLBM.initializeCells();

	//Setting solids for LBM simulation:
	setDomain();

	//Initial cell condition
	eIMB.eLBM.setInitCond(rhoInit, velInit);
}

void Scene::LBMEngine() {
	eIMB.eLBM.setZouBC();
	eIMB.eLBM.collision();
	eIMB.eLBM.setBounceBack();
	eIMB.eLBM.stream();
}

void Scene::DEMEngine() {
	eIMB.eDEM.contactVerification();
	eIMB.eDEM.forceCalculation();
	eIMB.eDEM.updateVelPos();
	eIMB.eDEM.updateContact();
}

void Scene::moveToNextTimeStep() {

	//Create directories
	int ignore = system("mkdir VTK_Fluid");
	    ignore = system("mkdir VTK_Solid");

	double tlbm = 0.0;
	int i = 0;
	while (Time < simDuration) {
		eIMB.defineLinkedCells();
		eIMB.calculateForceAndTorque();
		eIMB.eDEM.contactVerification();
		eIMB.eDEM.forceCalculation();
		eIMB.eDEM.updateVelPos();
		eIMB.eDEM.updateContact();
		if (Time >= tlbm) {
			eIMB.eLBM.collisionNT();
			eIMB.eLBM.setBounceBack();
			eIMB.eLBM.stream();
			tlbm += eIMB.eLBM.dtLBM;
		}
		Time += eIMB.eDEM.dtDEM;
		eIMB.eLBM.resetSolidFraction();
		/*setDomain();*/
		eIMB.updateFluidSolidContact();
		++i;
	}
}