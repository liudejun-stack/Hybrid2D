#ifndef DEM_H
#define DEM_H

#include <string>
#include <fstream>
#include <iostream>
#include "Interaction.hpp"


class DEM{
    public:
    
    void  demInfo();
    void  addBody(double _mass, double _radius, Vec2d _pos, Vec2d _vel);
    void  set_TimeStep(double _FoS, double _maxStiffness);
    void  set_Boundary(Vec2i _xLim, Vec2i _yLim);
    Vec2d applyBorderForce(std::shared_ptr<Body> _body);
    void  demEnergy();
    void  demCycle();
    void  outputECSV(int _nIter, std::string _fileName);
    void  outputSVTK(std::string _fileName);

    //Smart pointers to classes:
    std::vector<std::shared_ptr<Body>> bodies;
    std::vector<std::shared_ptr<Interaction>> interactions;

    //Vector:
    std::vector<double> kinEnergy;
    std::vector<double> potEnergy;

    //Model variables:
    Vec2i  xLim = Vec2i::Zero();
    Vec2i  yLim = Vec2i::Zero();
    Vec2d  gravity        = {0.0, -9.81};
    double localDamping   = 0.8;
    double frictionAngle  = 30;
    double borderStifness = 1e6;
    double dt             = 0.0;
    double time           = 0.0;
    int    vtkCounter     = 0;
    int    nIter          = 0;
};

#endif //DEM_H