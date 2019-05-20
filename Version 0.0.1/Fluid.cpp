#include "Fluid.hpp"
#include <iostream>

double Fluid::EqFun(double _Rho, double _ux, double _uy, int k) {
    double cdotu = D->cx[k]*_ux + D->cy[k]*_uy;
    double udotu = _ux*_ux + _uy*_uy;
    
    return D->w[k]*_Rho*(1 + 3*cdotu + 4.5*cdotu*cdotu - 1.5*udotu);
}

void Fluid::InitCond() {
    for (int j = 0; j <= G->Ny; j++){
        for (int i = 0; i <= G->Nx; i++){
            int id = G->MapGrid(i,j);
            rho[id] = RhoInit;
            ux[id]  = uxInit;
            uy[id]  = uyInit;
            
            for (int k = 0; k < D->Q; k++){
                int idf = G->MapFunction(i,j,k);
                f[idf] = EqFun(rho[id], ux[id], uy[id], k);
                //std::cout << rho[id] << " " << ux[id] << " " << uy[id] << " " << f[idf] << std::endl;
            }
        }
    }
}

void Fluid::Collision() {
    for (int j = 0; j <= G->Ny; j++){
        for (int i = 0; i <= G->Nx; i++){
            for (int k = 0; k < D->Q; k++){
                int id  = G->MapGrid(i,j);
                int idf = G->MapFunction(i,j,k);
                if (!G->Boundary[id]){
                    double EDF = EqFun(rho[id], ux[id], uy[id], k);     //Equi. Dist. Func.
                    fPos[idf] = f[idf] - (f[idf]-EDF)/tau;
                    //std::cout << fPos[idf] << " " << f[idf] << std::endl;
                }
            }
        }
    }
}

void Fluid::Stream() {
    for (int j = 0; j <= G->Ny; j++){
        for (int i = 0; i <= G->Nx; i++){
            for (int k = 0; k < D->Q; k++){
                int jd = j - D->cy[k];
                int id = i - D->cx[k];
                if (!G->Boundary[G->MapGrid(id,jd)]){
                    f[G->MapFunction(i,j,k)] = fPos[G->MapFunction(id,jd,k)];
                }
            }
        }
    }
}

void Fluid::BounceBack(){
  
    for (int j = 0; j <= G->Ny; j++){
        for (int i = 0; i <= G->Nx; i++){
            if (G->Boundary[G->MapGrid(i,j)]){
                double Temp;
                Temp = f[G->MapFunction(i,j,1)]; f[G->MapFunction(i,j,1)] = f[G->MapFunction(i,j,3)]; f[G->MapFunction(i,j,3)] = Temp;
                Temp = f[G->MapFunction(i,j,2)]; f[G->MapFunction(i,j,2)] = f[G->MapFunction(i,j,4)]; f[G->MapFunction(i,j,4)] = Temp;
                Temp = f[G->MapFunction(i,j,5)]; f[G->MapFunction(i,j,5)] = f[G->MapFunction(i,j,7)]; f[G->MapFunction(i,j,7)] = Temp;
                Temp = f[G->MapFunction(i,j,6)]; f[G->MapFunction(i,j,6)] = f[G->MapFunction(i,j,8)]; f[G->MapFunction(i,j,8)] = Temp;
            }
        }
    }
}

void Fluid::ZouHeBC()
{
   
}

void Fluid::MacroUpdate() {
    
    for (int j = 0; j <= G->Ny; j++){
        for (int i = 0; i <= G->Nx; i++){
            if (!G->Boundary[G->MapGrid(i,j)]) {
                rho[G->MapGrid(i,j)] = f[G->MapFunction(i,j,0)]+f[G->MapFunction(i,j,1)]+f[G->MapFunction(i,j,2)]+f[G->MapFunction(i,j,3)]+f[G->MapFunction(i,j,4)]+f[G->MapFunction(i,j,5)]+f[G->MapFunction(i,j,6)]+f[G->MapFunction(i,j,7)]+f[G->MapFunction(i,j,8)];
                
                ux[G->MapGrid(i,j)] = (f[G->MapFunction(i,j,1)] + f[G->MapFunction(i,j,5)] + f[G->MapFunction(i,j,8)] - f[G->MapFunction(i,j,3)] - f[G->MapFunction(i,j,6)] - f[G->MapFunction(i,j,7)])/rho[G->MapGrid(i,j)];
                
                ux[G->MapGrid(i,j)] = (f[G->MapFunction(i,j,5)] + f[G->MapFunction(i,j,6)] + f[G->MapFunction(i,j,2)] - f[G->MapFunction(i,j,7)] - f[G->MapFunction(i,j,8)] - f[G->MapFunction(i,j,4)])/rho[G->MapGrid(i,j)];
                
                //std::cout << rho[G->MapGrid(i,j)] << " " << ux[G->MapGrid(i,j)] << " " << uy[G->MapGrid(i,j)] << std::endl;
                
                if (isnan(rho[G->MapGrid(i,j)]) || isinf(rho[G->MapGrid(i,j)])){
                    std::cout << "ERROR: NAN or INF" << std::endl;
                }
                
                if (isnan(ux[G->MapGrid(i,j)]) || isinf(ux[G->MapGrid(i,j)])){
                    std::cout << "ERROR: NAN or INF" << std::endl;
                }
                
                if (isnan(uy[G->MapGrid(i,j)]) || isinf(uy[G->MapGrid(i,j)])){
                    std::cout << "ERROR: NAN or INF" << std::endl;
                }
            }
        }
    }
}

//RESCREVER ESTA FUNÇÃO!!
void Fluid::writeFluidVTK(std::string _Filename)
{
    int size = G->Nx * G->Ny;
	std::ofstream Output;
	Output.open(_Filename + std::to_string(vtkCounter) + ".vtk");

	Output << "# vtk DataFile Version 3.0\n";
	Output << "Fluid_state\n";
	Output << "ASCII\n";
	Output << "DATASET STRUCTURED_POINTS\n";
	Output << "DIMENSIONS " << G->Nx << " " << G->Ny << " " << 1 << "\n";
	Output << "ORIGIN "     << 0     << " " << 0     << " " << 0 << "\n";
    Output << "SPACING "    << 1     << " " << 1     << " " << 1 << "\n";
	Output << "POINT_DATA " << size << "\n";
    
    Output << "SCALARS Geom float 1\n";
    Output << "LOOKUP_TABLE default\n";
    for (int j = 0; j <= G->Ny-1; j++){
        for (int i = 0; i <= G->Nx-1; i++){
            int id = G->MapGrid(i,j);
            Output << G->Boundary[id] << "\n";
        }
    }
    
	Output << "SCALARS Density float 1\n";
	Output << "LOOKUP_TABLE default\n";
	for (int j = 0; j <= G->Ny-1; j++) {
		for (int i = 0; i <= G->Nx-1; i++) {
			Output << rho[G->MapGrid(i, j)] << "\n";
		}
	}
	
	Output << "VECTORS Velocity float\n" << std::endl;
	for (int j = 0; j <= G->Ny-1; j++) {
		for (int i = 0; i <= G->Nx-1; i++) {
			Output << ux[G->MapGrid(i,j)] << " " << uy[G->MapGrid(i,j)] << 0 << "\n";
		}
	}
	Output.close();
	vtkCounter++;
}


void Fluid::solve(int nIter, std::string _Filename)
{
    G->setBoundary(true, true, false, false);
    InitCond();
    for(int i = 0; i != nIter; i++){
        std::cout << i << std::endl;
        Collision();
        Stream();
        BounceBack();
        //ZouHeBC();
        MacroUpdate();
        if(i % 100 == 0)   writeFluidVTK(_Filename);
    }
}


