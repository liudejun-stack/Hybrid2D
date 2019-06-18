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
    
}