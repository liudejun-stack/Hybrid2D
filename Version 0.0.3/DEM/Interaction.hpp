#ifndef INTERACTION_H
#define INTERACTION_H
#include "Body.hpp"
#include <memory>

class Interaction{
    public:
    Interaction(std::weak_ptr<Body> _body1, std::weak_ptr<Body> _body2) : body1(_body1), body2(_body2){};

    //Methods:
    bool checkContact();
    void set_UnitVectorandContact();
    void set_ForceAndShearIncrements(double _dt);
    void applyFrictionLaw();

    //Smart pointers:
    std::weak_ptr<Body> body1;
    std::weak_ptr<Body> body2;

    //Variables:
    Vec2d unitNormal   = Vec2d::Zero();
    Vec2d unitShear    = Vec2d::Zero();
    Vec2d contact      = Vec2d::Zero();
    double normalForce = 0.0;
    double shearForce  = 0.0;

    //Friction parameters:
    double phi         = 30;
    double kn          = 1e6;
    double ks          = 0.5e6;
};

#endif //INTERACTION_H