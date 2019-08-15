#include "Scene.h"
#include <cmath>

void Scene::addBody(double _mass, double _radius, Vec2d _pos, Vec2d _vel) {
	int id = particle.bodies.size();
	particle.bodies.push_back(std::make_shared<Body>(_mass, _radius, _pos, _vel, id));
}

void Scene::setTimeStep(double _FoS, double _maxStiffness) {
	double maxMass = 0.0;
	for (auto& B : particle.bodies) {
		if (B->mass > maxMass)	maxMass = B->mass;
	}
	particle.dtCrit = std::sqrt(maxMass / _maxStiffness);
	particle.dt = _FoS * particle.dtCrit;
}

void Scene::setSolidFraction() {
	for (auto& B : particle.bodies) {
		for (auto& C : fluid.cells) {
			double distanceCenter = std::sqrt((B->pos - C->cellPos).norm());
			if (distanceCenter >= (B->radius + dx)) {
				C->solidFraction = 1.0;
			}
			else if (distanceCenter <= B->radius - dx) {
				C->solidFraction = 0.0;
			}
			else {
				double distanceSurface = distanceCenter - B->radius;
				C->solidFraction = -distanceSurface + B->functionR;
			}
			if (C->Boundary == true)	C->solidFraction = 1.0;
			if (C->solidFraction < 0.0)	C->solidFraction = 0.0;
			if (C->solidFraction > 1.0)	C->solidFraction = 1.0;
			ASSERT(C->solidFraction >= 0.0 && C->solidFraction <= 1.0);
			//print(C->solidFraction);
		}
	}
}

void Scene::calculateLBMForce() {
	for (auto& B : particle.bodies) {
		for (auto& C : fluid.cells) {
			for (int k = 0; k < C->Q; k++) {
				double feqOp = C->set_eqFun(C->rho, C->vel, C->opNode[k]);
				double feqPar = C->set_eqFun(C->rho, B->vel, k);
				C->omega[k] = (C->f[C->opNode[k]] - feqOp) - (C->f[k] - feqPar);
			}
		}
	}
}

void Scene::solve(std::string _fileName, int _nIter) {
	for (int i = 0; i != _nIter; i++) {
		print(i);
		setSolidFraction();
		fluid.updateMacro();
		calculateLBMForce();
		fluid.collision();
		fluid.stream();
		fluid.bounceback();
		if (i % 100 == 0)	fluid.outputFVTK(_fileName);
	}
}
