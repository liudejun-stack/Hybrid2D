#include "DEM.h"

void DEM::calculateParticleTimeStep() {
	double minMass = 1.0;
	for (auto& B : bodies) {
		if (B->mass < minMass)	minMass = B->mass;
	}
	dtDEM = factorOfSafety * std::sqrt(minMass / normalStiffness);
	ASSERT(dtDEM > 0.0 && minMass > 0.0);
}

Vector2r DEM::applyBorderForce(std::shared_ptr<Body> _body) {
	//Vectors:
	Vector2r unitUp = { 0, 1 };
	Vector2r unitDown = { 0,-1 };
	Vector2r unitRight = { 1, 0 };
	Vector2r unitLeft = { -1, 0 };
	Vector2r force = Vector2r::Zero();
	if ((_body->pos[0] - _body->radius) < domainReference[0]) force += borderStifness * abs(_body->radius - _body->pos[0]) * unitRight;
	if ((_body->pos[0] + _body->radius) > domainSize[0])      force += borderStifness * abs(_body->radius - (domainSize[0] - _body->pos[0])) * unitLeft;
	if ((_body->pos[1] - _body->radius) < domainReference[1]) force += borderStifness * abs(_body->radius - _body->pos[1]) * unitUp;
	if ((_body->pos[1] + _body->radius) > domainSize[1])      force += borderStifness * abs(_body->radius - (domainSize[1] - _body->pos[1])) * unitDown;
	return force;
}

void DEM::contactVerification() {
	int bodySize = (int)bodies.size();
	ASSERT(bodySize > 0);
	//Brute force method:
	for (int i = 0; i < bodySize - 1; ++i) {
		for (int j = i + 1; j < bodySize; ++j) {
			if (bodies[i]->checkInteraction(bodies[j]->id))	continue;
			if ((bodies[i]->pos - bodies[j]->pos).norm() < bodies[i]->radius + bodies[j]->radius) {
				interactions.push_back(std::make_shared<Interaction>(bodies[i], bodies[j]));
				bodies[i]->inter.push_back(interactions[interactions.size() - 1]);
				bodies[j]->inter.push_back(interactions[interactions.size() - 1]);
			}
		}
	}
}

void DEM::forceCalculation() {
	//Loop through interactions:
	for (auto& I : interactions) {
		I->calculateUnitVectorandContact();
		I->calculateForceAndShearIncrements(dtDEM, normalStiffness, shearStiffness);
		I->applyFrictionLaw(frictionAngle);
	}

	//Force calculation:
	for (auto& B : bodies) {
		//Force Resetter:
		B->force = Vector2r::Zero();
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
				B->moment += Il->shearForce * (Il->contact - B->pos).norm();
			}
		}

		//Body, fluid and border force:
		B->force += B->mass * gravity;
		B->force += applyBorderForce(B);
		B->force += B->forceLBM;
	}
}

void DEM::updateVelPos() {
	for (auto& B : bodies) {

		//Calculate accelaration from forces:
		Vector2r  linAccel = Vector2r::Zero();
		Vector2r  f = B->force;
		double    m = B->moment;
		double    rotAccel = 0.0;
		int       signV = 0;
		int       signM = 0;

		for (int i = 0; i < linAccel.size(); ++i) {
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
	++nIter;
	ASSERT(nIter > 0);
}

void DEM::updateContact() {
	interactions.erase(std::remove_if(std::begin(interactions), std::end(interactions), [](std::shared_ptr<Interaction> I) {return !I->checkContact(); }), std::end(interactions));
	for (auto& B : bodies) {
		B->inter.erase(std::remove_if(std::begin(B->inter), std::end(B->inter), [](std::weak_ptr<Interaction> I) {return I.expired(); }), std::end(B->inter));
	}
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