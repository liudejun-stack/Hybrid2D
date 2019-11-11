#include "Output.h"

//void Output::displaySimulationInfo() {
//	Scene& S = S.getScene();
//	std::system("cls");
//	S.eIMB.eDEM.calculateEnergy();
//	double totalEnergy = S.eIMB.eDEM.kinEnergy.back() + S.eIMB.eDEM.potEnergy.back();
//
//	std::cout << "----------------------- LBM/DEM Simulation --------------------------------" << "\n";
//	std::cout << "    Simulation Time: " << std::setprecision(3) << S.Time << "/" << S.simDuration << "\n";
//	std::cout << "        Domain Size: " << S.domainSize[0] << "x" << S.domainSize[1] << "\n";
//	std::cout << "   Number of Bodies: " << S.eIMB.eDEM.bodies.size() << "\n";
//	std::cout << "    Number of cells: " << S.eIMB.eLBM.cells.size() << "\n";
//
//	std::cout << "----------------------- LBM Parameters ------------------------------------" << "\n";
//	std::cout << "          Time Step: " << S.eIMB.eLBM.dtLBM << "\n";
//	std::cout << "    Lattice Spacing: " << S.eIMB.eLBM.dx << "\n";
//	std::cout << "    Relaxation Time: " << S.relaxationTime << "\n";
//	std::cout << "Kinematic Viscosity: " << S.kinViscosity << "\n";
//
//	std::cout << "----------------------- DEM Parameters ------------------------------------" << "\n";
//	std::cout << "          Time Step: " << S.eIMB.eDEM.dtDEM << "\n";
//	std::cout << "    SubCycle Number: " << S.subCycleNumber << "\n";
//	std::cout << "     Friction Angle: " << S.frictionAngle << "\n";
//	std::cout << "   Normal Stiffness: " << S.normalStiffness << "\n";
//	std::cout << "    Shear Stiffness: " << S.shearStiffness << "\n";
//	std::cout << "      Local Damping: " << S.localDamping << "\n";
//	std::cout << "   Kinematic Energy: " << S.eIMB.eDEM.kinEnergy.back() << "\n";
//	std::cout << "   Potential Energy: " << S.eIMB.eDEM.potEnergy.back() << "\n";
//	std::cout << "       Total Energy: " << totalEnergy << "\n";
//}