#include "DEM.h"

void DEM::addBody(double _mass, double _radius, Vec2d _pos, Vec2d _vel) {
	int id = bodies.size();
	bodies.push_back(std::make_shared<Body>(_mass, _radius, _pos, _vel, id));
}

void DEM::calculateParticleTimeStep() {
	double minMass = 1.0;
	for (auto& B : bodies) {
		if (B->mass < minMass)	minMass = B->mass;
	}
	ASSERT(minMass > 0);
	dtDEM = 2 * factorOfSafety * std::sqrt(minMass / kn);
}

Vec2d DEM::applyBorderForce(std::shared_ptr<Body> _body) {
	//Vectors:
	Vec2d unitUp = { 0, 1 };
	Vec2d unitDown = { 0,-1 };
	Vec2d unitRight = { 1, 0 };
	Vec2d unitLeft = { -1, 0 };
	Vec2d force = Vec2d::Zero();
	if ((_body->pos[0] - _body->radius) < domainReference[0]) force += borderStifness * abs(_body->radius - _body->pos[0]) * unitRight;
	if ((_body->pos[0] + _body->radius) > domainSize[0]) force += borderStifness * abs(_body->radius - (domainSize[0] - _body->pos[0])) * unitLeft;
	if ((_body->pos[1] - _body->radius) < domainReference[1]) force += borderStifness * abs(_body->radius - _body->pos[1]) * unitUp;
	if ((_body->pos[1] + _body->radius) > domainSize[1]) force += borderStifness * abs(_body->radius - (domainSize[1] - _body->pos[1])) * unitDown;
	return force;
}

void DEM::calculateEnergy() {
	double _kinEnergy = 0.0;
	double _potEnergy = 0.0;
	for (auto& B : bodies) {
		double v = B->vel.norm();
		double h = B->pos[1] - domainReference[1];
		_kinEnergy += B->mass * v * v / 2;
		_potEnergy += B->mass * abs(gravity[1]) * h;
	}
	kinEnergy.push_back(_kinEnergy);
	potEnergy.push_back(_potEnergy);
}

void DEM::demCycle() {
	int bodySize = (int)bodies.size();
	ASSERT(bodySize > 0);
	//Contact verification (Brute force method):
	for (int i = 0; i < bodySize - 1; i++)
		for (int j = i + 1; j < bodySize; j++) {
			if ((bodies[i]->pos - bodies[j]->pos).norm() < (bodies[i]->radius + bodies[j]->radius)) {
				interactions.push_back(std::make_shared<Interaction>(bodies[i], bodies[j]));
				bodies[i]->inter.push_back(interactions[interactions.size() - 1]);
				bodies[j]->inter.push_back(interactions[interactions.size() - 1]);
			}
		}

	//Loop through interactions:
	for (auto& I : interactions) {
		I->set_UnitVectorandContact();
		I->set_ForceAndShearIncrements(dtDEM, kn, ks);
		I->applyFrictionLaw(frictionAngle);
	}

	//Force calculation:
	for (auto& B : bodies) {
		B->force = Vec2d::Zero();
		B->moment = 0.0;

		//Contact force
		for (auto& I : B->inter) {
			auto Il = I.lock();
			auto iBody1 = Il->body1.lock();
			auto iBody2 = Il->body2.lock();
			if (B->id == iBody1->id) {
				B->force += -Il->normalForce * Il->unitNormal;
				B->force += -Il->shearForce * Il->unitShear;
				B->moment += Il->shearForce * (Il->contact - B->pos).norm();
			}
			if (B->id == iBody2->id) {
				B->force += Il->normalForce * Il->unitNormal;
				B->force += Il->shearForce * Il->unitShear;
				B->moment += -Il->shearForce * (Il->contact - B->pos).norm();
			}
		}

		//Body and border force:
		B->force += B->mass * gravity;
		B->force += applyBorderForce(B);
		B->force += B->forceLBM;

		//Calculate accelaration from forces:
		Vec2d  linAccel = { 0.0, 0.0 };
		Vec2d  f = B->force;
		double m = B->moment;
		double rotAccel = 0.0;
		int    signV;
		int    signM;

		for (int i = 0; i < linAccel.size(); i++) {
			B->vel[i] > 0 ? signV = 1 : signV = -1;
			linAccel[i] = ((f[i] - localDamping * abs(f[i]) * signV) / B->mass) * B->blockedDOFs[i];
		}

		B->rotVel > 0 ? signM = 1 : signM = -1;
		rotAccel = ((m - localDamping * abs(m) * signM) / B->inertiaMoment) * B->blockedMDOFs;

		//Update velocity and position (LeapFrog method):
		if (nIter == 0) {
			B->vel += linAccel * dtDEM * 0.5;
			B->rotVel += rotAccel * dtDEM * 0.5;
		}
		else {
			B->vel += linAccel * dtDEM;
			B->rotVel += rotAccel * dtDEM;
		}
		B->pos += B->vel * dtDEM;
		B->rot += B->rotVel * dtDEM;
	}
	//Interaction loss removal
	interactions.erase(std::remove_if(std::begin(interactions), std::end(interactions), [](std::shared_ptr<Interaction> I) {return !I->checkContact(); }), std::end(interactions));
	for (auto& B : bodies) {
		B->inter.erase(std::remove_if(std::begin(B->inter), std::end(B->inter), [](std::weak_ptr<Interaction> I) {return I.expired(); }), std::end(B->inter));
	}

	++nIter;
	time += dtDEM;
}

void DEM::energyCSV(std::string _fileName) {
	int j;
	double t = 0.0;
	std::ofstream out;
	out.open(_fileName + ".csv");
	out << "Time" << "," << "kinEnergy" << "," << "potEnergy" << std::endl;
	for (int i = 0; i < kinEnergy.size(); ++i) {
		i == 0 ? t = 0.0 : t += dtDEM;
		out << t << "," << kinEnergy[i] << "," << potEnergy[i] << std::endl;
	}
}

void DEM::particleVTK(std::string _fileName) {
	std::ofstream out;
	out.open(_fileName + std::to_string(vtkCounter) + ".vtk");
	out << "# vtk DataFile Version 3.0\n";
	out << "DEM\n";
	out << "ASCII\n";
	out << " \n";
	out << "DATASET POLYDATA\n";
	out << "POINTS " << std::to_string((int)bodies.size()) << " float\n";
	for (auto& B : bodies) {
		out << B->pos[0] << " " << B->pos[1] << " " << 0 << std::endl;
	}
	out << "POINT_DATA " << std::to_string((int)bodies.size()) << std::endl;
	out << "SCALARS radii double\n";
	out << "LOOKUP_TABLE default\n";
	for (auto& B : bodies) {
		out << B->radius << std::endl;
	}
	out.close();
	vtkCounter++;
}

