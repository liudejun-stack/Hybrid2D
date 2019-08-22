#include "IMB.h"
#include <cmath>

void IMB::addBody(double _mass, double _radius, Vec2d _pos, Vec2d _vel) {
	int id = particle.bodies.size();
	particle.bodies.push_back(std::make_shared<Body>(_mass, _radius, _pos, _vel, id));
}

void IMB::setTimeStep(double _FoS, double _kinematicViscosity) {
	double maxMass = 0.0;
	double maxStiffness = 0.0;
	for (auto& B : particle.bodies) {
		if (B->mass > maxMass)	maxMass = B->mass;
		if (particle.kn > maxStiffness)	maxStiffness = particle.kn;
	}
	particle.dt = _FoS * std::sqrt(maxMass / maxStiffness);
	fluid.dt = (1.0 / 3.0) * (fluid.tau - 0.5) * (fluid.dx * fluid.dx / _kinematicViscosity);

	fluid.dx = std::sqrt((3.0 * particle.dt * _kinematicViscosity) / (fluid.tau - 0.5));
	print(fluid.dx);
	
	for (auto& C : fluid.cells) {
		C->latticeSpeed = fluid.dx / particle.dt;
	}
	
}

void IMB::setSolidFraction() {
	for (auto& B : particle.bodies) {
		for (auto& C : fluid.cells) {
			double distanceCenter = std::sqrt((B->pos - C->cellPos).norm());
			if (distanceCenter >= (B->radius + fluid.dx)) {
				C->solidFraction = 1.0;
			}
			else if (distanceCenter <= B->radius - fluid.dx) {
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

void IMB::calculateHydroForce() {
	for (auto& B : particle.bodies) {
		B->forceLBM = Vec2d::Zero();
		for (auto& C : fluid.cells) {
			for (int k = 0; k < C->Q; k++) {
				double feqOp = C->set_eqFun(C->rho, C->vel, C->opNode[k]);
				double feqPar = C->set_eqFun(C->rho, B->vel, k);
				C->omega[k] = (C->f[C->opNode[k]] - feqOp) - (C->f[k] - feqPar);
			}

			C->solidFunction = (C->solidFraction * (fluid.tau - 0.5)) / ((1 - C->solidFraction) + fluid.tau - 0.5);
			ASSERT(C->solidFunction >= 0.0 && C->solidFunction <= 1.0);

			Vec2d soma = Vec2d::Zero();
			for (int k = 0; k < C->Q; k++) {
				soma += C->omega[k] * C->discreteVelocity[k];
			}
			B->forceLBM = C->latticeSpeed*fluid.dx*C->solidFunction * soma;
		}
	}
}

void IMB::solve(std::string _fileName, int _nIter) {
	for (int i = 0; i != _nIter; i++) {
		print(i);
		fluid.updateMacro();
		setSolidFraction();
		calculateHydroForce();
		fluid.collision();
		fluid.stream();
		fluid.bounceback();
		particle.demCycle();
		if (i % 100 == 0) {
			fluid.outputFVTK(_fileName);
			particle.outputSVTK("DEM");
		}
	}
}
