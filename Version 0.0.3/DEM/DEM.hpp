#ifndef DEM_H
#define DEM_H

#include <string>
#include "Interaction.hpp"


class DEM{
    public:
    
    void  addBody(double _mass, double _radius, Vec2d _pos, Vec2d _vel);
    void  setTimeStep(double _FoS, double _maxMass, double _maxStiffness);
    void  setBoundary(double _xoLim, double _xfLim, double _yoLim, double _yfLim);
    Vec2d applyBorderForce(std::shared_ptr<Body> _body);
    void  demCycle();
    void  outputSVTK(std::string _fileName);
    void  solve(int _nIter, std::string _fileName);

    //Smart pointers to classes:
    std::vector<std::shared_ptr<Body>> bodies;
    std::vector<std::shared_ptr<Interaction>> interactions;

    //Model variables:
    Vec2d  gravity        = {0.0, -9.81};
    double localDamping   = 0.8;
    double frictionAngle  = 30;
    double borderStifness = 1e6;
    double dt             = 0.0;
    double time           = 0.0;
    int    vtkCounter     = 0;

};

#endif //DEM_H