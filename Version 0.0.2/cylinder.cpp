#include "Fluid.hpp"

double uMax   = 0.1;
double Re     = 5;
int    nx     = 100;
int    ny     = 100;
int    radius = ny/20 + 1;


double calcVisc() { 

    double kinVisc = uMax * (2*radius)/Re;
    return 3.0*kinVisc + 0.5;
}

void calcInitSpeed(int _x, int _y, double& vx, double& vy){
    double L = ny - 2;
    double yp = _y - 1.5;
    vx = uMax*4/(L*L)*(L*yp - yp*yp);
    vy = 0.0;
}


int main(int argc, char **argv) {
/*
    int obsX = ny/2;
    int obsY = ny/2 + 3;

    Fluid F;
    F.setTau(1.0);
    F.D->setBoundary(true, true, false, false);
    F.setObstacle(obsX, obsY, radius);
    
    for (int j = 0; j < F.D->ny; j++){
        double vx, vy;
        calcInitSpeed(0,j,vx,vy);
        F.setVelBC(0,j,vx,vy);
        F.setDensBC(nx-1,j,1.0);
    }
    */
    Fluid F;
    F.setInitCond(1.0, 0.08, 0.0);
    //F.solve(1000, "Resultado");

}
