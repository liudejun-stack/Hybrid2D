#include "Scene.h"

void Scene::print() {
	std::cout << L.cells.size() << std::endl;
	std::cout << D.bodies.size() << std::endl;
	std::cout << D.dt << std::endl;
}

void Scene::addBody(double _mass, double _radius, Vec2d _pos, Vec2d _vel) {
	int id = D.bodies.size();
	D.bodies.push_back(std::make_shared<Body>(_mass, _radius, _pos, _vel, id));
}

void Scene::setTimeStep(double _FoS, double _maxStiffness) {
	double maxMass = 0.0;
	for (auto& B : D.bodies) {
		if (B->mass > maxMass)	maxMass = B->mass;
	}
	D.dtCrit = std::sqrt(maxMass / _maxStiffness);
	D.dt = _FoS * D.dtCrit;

}