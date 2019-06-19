#include "DEM.hpp"

int main(){
	DEM D;
	D.addBody(1, 0.5, {5,5}, {1.5, 1});
    D.addBody(1, 0.5, {3,2}, {1.5, 1.5});
    D.set_TimeStep(0.5, 1, 1e6);
    Vec2i xLim = {0, 10};
    Vec2i yLim = {0, 10};
    D.set_Boundary(xLim, yLim);
    D.localDamping = 0.1;
    D.gravity = {0, -9.81};
    D.frictionAngle = 30;
    
    for (int i = 0; i != 30000; i++){
        D.demCycle();
        if(i%100 == 0)  D.outputSVTK("resultado");
    }
}
