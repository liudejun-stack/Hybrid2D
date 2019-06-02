#include "Fluid.hpp"
#include <iostream>

void Fluid::setTau(double _tau) {
    tau = _tau;
}

double Fluid::setEqFun(double _rho, double _ux, double _uy, int k) {
    double cdotu = D->cx[k]*_ux + D->cy[k]*_uy;
    double udotu = _ux*_ux + _uy*_uy;
    return D->w[k]*_rho*(1 + 3*cdotu + 4.5*cdotu*cdotu - 1.5*udotu);
}

void Fluid::setInitCond(double _rhoInit, double _uxInit, double _uyInit) {
    for (int j = 0; j < D->ny; j++)
    for (int i = 0; i < D->nx; i++){
        int id = D->MapGrid(i,j);
        rho[id] = _rhoInit;
        ux[id]  = _uxInit;
        uy[id]  = _uyInit;
        for (int k = 0; k < D->Q; k++){
            int idf = D->MapFunction(i,j,k);
            f[idf] = setEqFun(rho[id], ux[id], uy[id], k);
        }
    }
}

void Fluid::setVelBC(int i, int j, double _ux, double _uy) {
    int id = D->MapGrid(i,j);
    ux[id] = _ux;
    uy[id] = _uy;
}

void Fluid::setDensBC(int i, int j, double _rho) {
    int id = D->MapGrid(i,j);
    rho[id] = _rho;
}

void Fluid::setObstacle(int _obsX, int _obsY, int _radius) {
    for (int j = 0; j < D->ny; j++)
    for (int i = 0; i < D->nx; i++){
        if (((i-_obsX)*(i-_obsX) + (j - _obsY)*(j - _obsY)) <= _radius*_radius) {
            D->Boundary[D->MapGrid(i,j)] = isSolid;
        }
    }
}

double Fluid::Density(){
    for (int j = 0; j < D->ny; j++)
    for (int i = 0; i < D->nx; i++){
        int id = D->MapGrid(i,j);
        if (D->Boundary[id]==isSolid){
            return 0.0;
        }
        else{
            double rho = 0.0;
            for (int k = 0; k < D->Q; k++){
                int idf = D->MapFunction(i,j,k);
                rho += f[idf];
            }
            return rho;
        }
    }
}

void Fluid::Velocity(){
    for (int j = 0; j < D->ny; j++)
    for (int i = 0; i < D->nx; i++){
        int id = D->MapGrid(i,j);
        if (D->Boundary[id]==isSolid){
            uxMacro = 0.0;
            uyMacro = 0.0;
        }
        uxMacro = 0.0; 
        uyMacro = 0.0;
        rho = Density();
        for (int k = 0; k < D->Q; k++){
            uxMacro += (f[D->MapFuncion(i,j,k)]*D->cx[k])/rho;
            uyMacro += (f[D->MapFuncion(i,j,k)]*D->cx[k])/rho;
        }
    }
}

void Fluid::Collision() {
    double tauInv = D->dt/tau;
    for (int j = 0; j < D->ny; j++)
    for (int i = 0; i < D->nx; i++)
    for (int k = 0; k < D->Q;  k++){
        int id  = D->MapGrid(i,j);
        int idf = D->MapFunction(i,j,k);
        rhoMacro = Density();
        if (D->Boundary[id]==notSolid) {
            double EDF = setEqFun(rhoMacro,uxMacro, uyMacro, k);
            f[idf] = (1 - tauInv)*f[idf] + tauInv*EDF;
        }
    }
}

void Fluid::Stream() {
    for (int j = 0; j < D->ny; j++)
    for (int i = 0; i < D->nx; i++)
    for (int k = 0; k < D->Q;  k++){
        int idf = D->MapFunction(i,j,k);
        int idx_i = (i + D->cx[k]) % D->nx;
        int idx_j = (j + D->cy[k]) % D->ny;
        fTemp[D->MapFunction(idx_i, idx_j, k)] = f[idf];
    }

//Swap distribuition function
    for (int j = 0; j < D->ny; j++)
    for (int i = 0; i < D->nx; i++)
    for (int k = 0; k < D->Q;  k++){
        int idf = D->MapFunction(i,j,k);
        f[idf] =  fTemp[idf];
    }
    
    for (int j = 0; j < D->ny; j++)
    for (int i = 0; i < D->nx; i++){
        int id = D->MapGrid(i,j);
        Velocity();
        rho[id] = Density();
        ux[id] = uxMacro;
        uy[id] = uyMacro;
    }
}

void Fluid::BounceBack(){

    for (int j = 0; j < D->ny; j++){
        for (int i = 0; i < D->nx; i++){
            for (int k = 0; k < D->Q; k++){
                int id = D->MapGrid(i,j);
                int idf = D->MapFunction(i,j,k);
                if (D->Boundary[id]){
                    fTemp[idf] = f[idf];
                }   
            }
        }
    }

   for (int j = 0; j < D->ny; j++){
        for (int i = 0; i < D->nx; i++){
            for (int k = 0; k < D->Q; k++){
                int id = D->MapGrid(i,j);
                int idf = D->MapFunction(i,j,k);
                if (D->Boundary[id]){
                    f[idf] = fTemp[D->MapFunction(i,j,op[k])];
                }   
            }
        }
    } 
}

void Fluid::ZouHeBC() 
{
    //Prescribed velocity:
    //Left side
    for (int j = 0; j < D->ny; j++){
        int i = 0;
        if (!D->Boundary[D->MapGrid(i,j)]){

            double rho = (f[D->MapFunction(i,j,0)] + f[D->MapFunction(i,j,2)] + f[D->MapFunction(i,j,4)] + 
            2.0*(f[D->MapFunction(i,j,3)]+f[D->MapFunction(i,j,6)]+f[D->MapFunction(i,j,7)])) / (1.0 - ux[D->MapGrid(i,j)]);

            f[D->MapFunction(i,j,1)] = f[D->MapFunction(i,j,3)] + (2.0/3.0)*rho*ux[D->MapGrid(i,j)];

            f[D->MapFunction(i,j,5)] = f[D->MapFunction(i,j,7)] + (1.0/6.0)*rho*ux[D->MapGrid(i,j)] +
            0.5*rho*uy[D->MapGrid(i,j)] - 0.5*(f[D->MapFunction(i,j,2)] - f[D->MapFunction(i,j,4)]);

            f[D->MapFunction(i,j,8)] = f[D->MapFunction(i,j,6)] + (1.0/6.0)*rho*ux[D->MapGrid(i,j)] -
            0.5*rho*uy[D->MapGrid(i,j)] + 0.5*(f[D->MapFunction(i,j,2)] - f[D->MapFunction(i,j,4)]);
        }
    }

    //Right Side:
    for (int j = 0; j < D->ny; j++){
        int i = D->nx-1;
        if (!D->Boundary[D->MapGrid(i,j)]){

            double rho = (f[D->MapFunction(i,j,0)] + f[D->MapFunction(i,j,2)] + f[D->MapFunction(i,j,4)] + 
            2.0*(f[D->MapFunction(i,j,1)]+f[D->MapFunction(i,j,5)]+f[D->MapFunction(i,j,8)])) / (1.0 + ux[D->MapGrid(i,j)]);

            f[D->MapFunction(i,j,3)] = f[D->MapFunction(i,j,1)] - (2.0/3.0)*rho*ux[D->MapGrid(i,j)];

            f[D->MapFunction(i,j,7)] = f[D->MapFunction(i,j,5)] - (1.0/6.0)*rho*ux[D->MapGrid(i,j)] + 
            0.5*(f[D->MapFunction(i,j,2)] - f[D->MapFunction(i,j,4)]);

            f[D->MapFunction(i,j,6)] = f[D->MapFunction(i,j,8)] - (1.0/6.0)*rho*ux[D->MapGrid(i,j)] - 
            0.5*(f[D->MapFunction(i,j,2)] - f[D->MapFunction(i,j,4)]);
        }
    }

    //Prescribed density:
    //Right side
    for (int j = 0; j < D->ny; j++){
        int i = D->nx-1;
        if (!D->Boundary[D->MapGrid(i,j)]){

            double vx = -1.0 + (f[D->MapFunction(i,j,0)] + f[D->MapFunction(i,j,2)] + f[D->MapFunction(i,j,4)] + 
            2.0*(f[D->MapFunction(i,j,1)] + f[D->MapFunction(i,j,5)] + f[D->MapFunction(i,j,8)])) / rho[D->MapGrid(i,j)];

            f[D->MapFunction(i,j,3)] = f[D->MapFunction(i,j,1)] - (2.0/3.0)*rho[D->MapGrid(i,j)]*vx;

            f[D->MapFunction(i,j,7)] = f[D->MapFunction(i,j,5)] - (1.0/6.0)*rho[D->MapGrid(i,j)]*vx +
            0.5*(f[D->MapFunction(i,j,2)] + f[D->MapFunction(i,j,4)]);

            f[D->MapFunction(i,j,6)] = f[D->MapFunction(i,j,8)] - (1.0/6.0)*rho[D->MapGrid(i,j)]*vx -
            0.5*(f[D->MapFunction(i,j,2)] + f[D->MapFunction(i,j,4)]);
        }
    }
}

void Fluid::writeFluidVTK(std::string _Filename)
{
    int size = D->nx * D->ny;
	std::ofstream Output;
	Output.open(_Filename + std::to_string(vtkCounter) + ".vtk");

	Output << "# vtk DataFile Version 3.0\n";
	Output << "Fluid_state\n";
	Output << "ASCII\n";
	Output << "DATASET STRUCTURED_POINTS\n";
	Output << "DIMENSIONS " << D->nx << " " << D->ny << " " << 1 << "\n";
	Output << "ORIGIN "     << 0     << " " << 0     << " " << 0 << "\n";
    Output << "SPACING "    << 1     << " " << 1     << " " << 1 << "\n";
	Output << "POINT_DATA " << size << "\n";
    
    Output << "SCALARS Geom float 1\n";
    Output << "LOOKUP_TABLE default\n";
    for (int j = 0; j < D->ny; j++){
        for (int i = 0; i < D->nx; i++){
            int id = D->MapGrid(i,j);
            Output << D->Boundary[id] << "\n";
        }
    }
    
	Output << "SCALARS Density float 1\n";
	Output << "LOOKUP_TABLE default\n";
	for (int j = 0; j < D->ny; j++) {
		for (int i = 0; i < D->nx; i++) {
			Output << rho[D->MapGrid(i, j)] << "\n";
		}
	}
	
	Output << "VECTORS Velocity float\n" << std::endl;
	for (int j = 0; j < D->ny; j++) {
		for (int i = 0; i < D->nx; i++) {
			Output << ux[D->MapGrid(i,j)] << " " << uy[D->MapGrid(i,j)] << " " << 0 << "\n";
		}
	}
	Output.close();
	vtkCounter++;
}

void Fluid::solve(int nIter, std::string _Filename)
{
    for(int i = 0; i != nIter; i++){
        std::cout << i << std::endl;
        MacroUpdate();
        ZouHeBC();
        Collision();
        BounceBack();
        Stream();
        if(i % 100 == 0)   writeFluidVTK(_Filename);
    }
}


