#include "Interaction.hpp"
#include <math.h>

bool Interaction::checkContact(){
    auto b1 = body1.lock();
    auto b2 = body2.lock();
    if((b1->pos - b2->pos).norm() < b1->radius + b2->radius) {
        return true;
    }
    return false;
}

void Interaction::set_UnitVectorandContact(){
    auto b1 = body1.lock();
    auto b2 = body2.lock();

    unitNormal = (b1->pos - b2->pos) / (b1->pos - b2->pos).norm();
    unitShear  = {unitNormal[1], -unitNormal[0]};
    contact    = (b1->pos + b2->pos)*0.5;
}

void Interaction::set_ForceAndShearIncrements(double _dt){
    auto b1 = body1.lock();
    auto b2 = body2.lock();

    Vec2d RelVel;
    RelVel = (b1->vel - b2->vel) - (b1->rotVel*b1->radius + b2->rotVel*b2->radius)*unitShear;

    double normalIncrement = (RelVel.dot(unitNormal))*_dt;
    double shearIncrement  = (RelVel.dot(unitShear)) *_dt;

    normalForce += kn * normalIncrement;
    shearForce  += ks * shearIncrement;
}

void Interaction::applyFrictionLaw(){
    double maxShearForce = normalForce*tan(phi*M_PI/180);
    if(abs(shearForce) > maxShearForce){
        if(shearForce > 0)  shearForce =  maxShearForce;
        if(shearForce < 0)  shearForce = -maxShearForce;
    }
}