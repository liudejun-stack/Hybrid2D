#include "DEM.hpp"


void DEM::addBody(double _mass, double _radius, Vec2d _pos, Vec2d _vel){
    int _id = bodies.size();
    bodies.push_back(std::make_shared<Body>(_mass, _radius, _pos, _vel, _id));
}

void DEM::set_TimeStep(double _FoS, double _maxMass, double _maxStiffness){
    dt = _FoS*std::sqrt(_maxMass/_maxStiffness);
}

void DEM::set_Boundary(Vec2i _xLim, Vec2i _yLim){
    xLim = _xLim;
    yLim = _yLim;
}

Vec2d DEM::applyBorderForce(std::shared_ptr<Body> _body){
    //Vectors:
    Vec2i unitUp    = { 0, 1};
    Vec2i unitDown  = { 0,-1};
    Vec2i unitRight = { 1, 0};
    Vec2i unitLeft  = {-1, 0};
    if(_body->pos[0] < xLim[0]) force += borderStifness*abs(xLim[0] - _body->pos[0])*unitRight;
    if(_body->pos[0] > xLim[1]) force += borderStifness*abs(xLim[1] - _body->pos[0])*unitLeft;
    if(_body->pos[1] < yLim[0]) force += borderStifness*abs(yLim[0] - _body->pos[1])*unitUp;
    if(_body->pos[1] > yLim[1]) force += borderStifness*abs(yLim[1] - _body->pos[1])*unitDown;
    return force;
}

void demCycle(){
    int bodySize = bodies.size();
    //Contact verification (Brute force method):
    for (int i = 0;   i < bodySize-1; i++)
    for (int j = i+1; j < bodySize;   j++){
        if((bodies[i]->pos - bodies[j]->pos).norm() < bodies[i]->radius + bodies[j]->radius){
            interactions.push_back(std::make_shared<Interaction>(bodies[i], bodies[j]));
            bodies[i]->inter.push_back(interactions[interactions.size()-1]);
            bodies[j]->inter.push_back(interactions[interactions.size()-1]);
        }
    }

    //Loop through interactions:
    for (auto& I : interactions){
        I->set_UnitVectorandContact();
        I->set_ForceAndShearIncrements(dt);
        I->applyFrictionLaw();
    }

    //Force calculation:
    for (auto& B : bodies){
        B->force  = Vec2d::Zero();
        B->moment = 0.0;

        //Contact force
        for (auto& I : inter){
            auto Il     = I.lock();
            auto iBody1 = Il->body1.lock();
            auto iBody2 = Il->body2.lock();
            if(B->id == iBody1->id){
                B->force  += -Il->normalForce * Il->unitNormal;
                B->force  += -Il->shearForce  * Il->unitShear;
                B->moment += shearForce * (Il->contact - B->pos).norm();
            }
            if(B->id == iBody2->id){
                B->force  += Il->normalForce * Il->unitNormal;
                B->force  += Il->shearForce  * Il->unitShear;
                B->moment += -shearForce * (Il->contact - B->pos).norm();
            }
        
        //Body and border force:
        B->force += B->mass*gravity;
        B->force += applyBorderForce(B);


        }
    }
}