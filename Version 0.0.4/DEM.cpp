#include "DEM.h"

void DEM::setBoundary(Vec2i _xLim, Vec2i _yLim) {
	xLim = _xLim;
	yLim = _yLim;
}

void DEM::setTimeStep(double _FoS, double _maxStiffness) {
	double maxMass = 0.0;
	for (auto& B : bodies) {
		if (B->mass > maxMass)   maxMass = B->mass;
	}
	ASSERT(maxMass > 0);
	dtCrit = std::sqrt(maxMass / _maxStiffness);
	dt = _FoS * dtCrit;
}

Vec2d DEM::applyBorderForce(std::shared_ptr<Body> _body) {
	//Vectors:
	Vec2d unitUp = { 0, 1 };
	Vec2d unitDown = { 0,-1 };
	Vec2d unitRight = { 1, 0 };
	Vec2d unitLeft = { -1, 0 };
	Vec2d force = Vec2d::Zero();
	if ((_body->pos[0] - _body->radius) < xLim[0]) force += borderStifness * abs(_body->radius - _body->pos[0]) * unitRight;
	if ((_body->pos[0] + _body->radius) > xLim[1]) force += borderStifness * abs(_body->radius - (xLim[1] - _body->pos[0])) * unitLeft;
	if ((_body->pos[1] - _body->radius) < yLim[0]) force += borderStifness * abs(_body->radius - _body->pos[1]) * unitUp;
	if ((_body->pos[1] + _body->radius) > yLim[1]) force += borderStifness * abs(_body->radius - (yLim[1] - _body->pos[1])) * unitDown;
	return force;
}

void DEM::demEnergy() {
	double _kinEnergy = 0.0;
	double _potEnergy = 0.0;
	for (auto& B : bodies) {
		double v = B->vel.norm();
		double h = B->pos[1] - yLim[0];
		_kinEnergy += B->mass * v * v / 2;
		_potEnergy += B->mass * abs(gravity[1]) * h;
	}
	kinEnergy.push_back(_kinEnergy);
	potEnergy.push_back(_potEnergy);
}

void DEM::demCycle() {
	int bodySize = (int)bodies.size();
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
		I->set_ForceAndShearIncrements(dt, kn, ks);
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
			B->vel += linAccel * dt * 0.5;
			B->rotVel += rotAccel * dt * 0.5;
		}
		else {
			B->vel += linAccel * dt;
			B->rotVel += rotAccel * dt;
		}
		B->pos += B->vel * dt;
		B->rot += B->rotVel * dt;
	}
	//Interaction loss removal
	interactions.erase(std::remove_if(std::begin(interactions), std::end(interactions), [](std::shared_ptr<Interaction> I) {return !I->checkContact(); }), std::end(interactions));
	for (auto& B : bodies) {
		B->inter.erase(std::remove_if(std::begin(B->inter), std::end(B->inter), [](std::weak_ptr<Interaction> I) {return I.expired(); }), std::end(B->inter));
	}

	++nIter;
	time += dt;
}

void DEM::outputECSV(std::string _fileName) {
	int j;
	double t = 0.0;
	std::ofstream out;
	out.open(_fileName + ".csv");
	out << "Time" << " " << "kinEnergy" << " " << "potEnergy" << std::endl;
	for (int i = 0; i < kinEnergy.size(); ++i) {
		i == 0 ? t = 0.0 : t += dt;
		out << t << " " << kinEnergy[i] << " " << potEnergy[i] << std::endl;
	}
}

void DEM::outputSVTK(std::string _fileName) {
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

