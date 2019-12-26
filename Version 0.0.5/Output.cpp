#include "Output.h"

void Output::displaySimulationInfo() {
	Scene& S = S.getScene();
	std::system("cls");
	S.eIMB.eDEM.calculateEnergy();
	double totalEnergy = S.eIMB.eDEM.kinEnergy.back() + S.eIMB.eDEM.potEnergy.back();

	std::cout << "----------------------- LBM/DEM Simulation --------------------------------" << "\n";
	std::cout << "    Simulation Time: " << std::setprecision(3) << S.Time << "/" << S.simDuration << "\n";
	std::cout << "        Domain Size: " << S.domainSize[0] << "x" << S.domainSize[1] << "\n";
	std::cout << "   Number of Bodies: " << S.eIMB.eDEM.bodies.size() << "\n";
	std::cout << "    Number of cells: " << S.eIMB.eLBM.cells.size() << "\n";

	std::cout << "----------------------- LBM Parameters ------------------------------------" << "\n";
	std::cout << "          Time Step: " << S.eIMB.eLBM.dtLBM << "\n";
	std::cout << "    Lattice Spacing: " << S.eIMB.eLBM.dx << "\n";
	std::cout << "    Relaxation Time: " << S.relaxationTime << "\n";
	std::cout << "Kinematic Viscosity: " << S.kinViscosity << "\n";

	std::cout << "----------------------- DEM Parameters ------------------------------------" << "\n";
	std::cout << "          Time Step: " << S.eIMB.eDEM.dtDEM << "\n";
	std::cout << "    SubCycle Number: " << S.subCycleNumber << "\n";
	std::cout << "     Friction Angle: " << S.frictionAngle << "\n";
	std::cout << "   Normal Stiffness: " << S.normalStiffness << "\n";
	std::cout << "    Shear Stiffness: " << S.shearStiffness << "\n";
	std::cout << "      Local Damping: " << S.localDamping << "\n";
	std::cout << "   Kinematic Energy: " << S.eIMB.eDEM.kinEnergy.back() << "\n";
	std::cout << "   Potential Energy: " << S.eIMB.eDEM.potEnergy.back() << "\n";
	std::cout << "       Total Energy: " << totalEnergy << "\n";
}

void Output::fluidVTK(std::string _fileName) {
	Scene& S = S.getScene();
	std::ofstream out;
	out.open("VTK_Fluid/" + _fileName + std::to_string(fluidVtkCounter) + ".vtk");
	out << "# vtk DataFile Version 3.0\n";
	out << "Fluid state\n";
	out << "ASCII\n";
	out << "DATASET STRUCTURED_POINTS\n";
	out << "DIMENSIONS " << S.domainSize[0] << " " << S.domainSize[1] << " " << 1 << "\n";
	out << "ORIGIN " << 0 << " " << 0 << " " << 0 << "\n";
	out << "SPACING " << 1 << " " << 1 << " " << 1 << "\n";
	out << "POINT_DATA " << S.domainSize[0] * S.domainSize[1] << "\n";
	out << "SCALARS Geometry float 1\n";
	out << "LOOKUP_TABLE default\n";
	for (auto& C : S.eIMB.eLBM.cells) {
		out << C->node << "\n";
	}
	out << "SCALARS Density float 1\n";
	out << "LOOKUP_TABLE default\n";
	for (auto& C : S.eIMB.eLBM.cells) {
		out << C->rho << "\n";
	}
	out << "VECTORS Velocity float\n";
	for (auto& C : S.eIMB.eLBM.cells) {
		out << C->vel[0] << " " << C->vel[1] << " " << 0 << "\n";
	}
	out.close();
	fluidVtkCounter++;
}

void Output::solidVTK(std::string _fileName) {
	Scene& S = S.getScene();
	std::ofstream out;
	out.open("VTK_Solid/" + _fileName + std::to_string(solidVtkCounter) + ".vtk");
	out << "# vtk DataFile Version 3.0\n";
	out << "DEM\n";
	out << "ASCII\n";
	out << "DATASET POLYDATA\n";
	out << "POINTS " << std::to_string((int)S.eIMB.eDEM.bodies.size()) << " double\n";
	for (const auto& B : S.eIMB.eDEM.bodies) {
		out << B->pos[0] << " " << B->pos[1] << " " << 0 << "\n";
	}
	out << "POINT_DATA " << std::to_string((int)S.eIMB.eDEM.bodies.size()) << std::endl;
	out << "SCALARS Radius double\n";
	out << "LOOKUP_TABLE default\n";
	for (auto& B : S.eIMB.eDEM.bodies) {
		out << B->radius << std::endl;
	}
	out.close();
	solidVtkCounter++;
}

void Output::fluidVelocityProfile(std::string _fileName, int _cellId) {
	Scene& S = S.getScene();
	std::shared_ptr<Lattice> cell = S.eIMB.eLBM.cells[_cellId];

	std::ofstream out;
	out.open("VTK_Fluid/" + std::to_string(fluidVtkCounter) + "_" + _fileName, std::ios_base::app);
	out << std::setprecision(3) << cell->ID << ", " << cell->cellPos[0] << ", " << cell->cellPos[1] << ", " 
		<< cell->vel[0] << ", " << cell->vel[1] << ", " << cell->vel.norm() << ", " << cell->rho << "\n";
	out.close();

}

void Output::particleEnergy(std::string _fileName, int _bodyId) {
	Scene& S = S.getScene();
	std::shared_ptr<Body> body = S.eIMB.eDEM.bodies[_bodyId];
	body->calculateEnergy();

	std::ofstream out;
	out.open("VTK_Solid/" + _fileName, std::ios_base::app);
	out << body->pos[0] << ", " << body->pos[1] << ", " << body->vel[0] << ", " << body->vel[1] << ", "
		<< body->vel.norm() << ", " << body->potEnergy << ", " << body->kinEnergy << "\n";
	out.close();
}