#include "Scene.h"

void Scene::arroz() {
	print(particle.bodies.size());
}

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

void Scene::lbmForce() {
	for (auto& B : particle.bodies) {
		for (auto& C : fluid.cells) {
			if (C->Boundary)	C->solidFraction = 1.0;
			double circle = (C->cellPos - B->pos).norm();
			if (circle <= B->radius * B->radius) {
				double distance = std::sqrt(circle);

			}
		}
	}
}

