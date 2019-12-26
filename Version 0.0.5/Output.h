#ifndef OUTPUT_H
#define OUTPUT_H

//Standard Library
#include <iomanip>
#include <fstream>
#include <string>
#include <memory>

//Hybrid Library
#include "Scene.h"
#include "Lattice.h"
#include "Body.h"

class Output {
public:
	void displaySimulationInfo();
	void fluidVTK(std::string _fileName);
	void solidVTK(std::string _fileName);
	void fluidVelocityProfile(std::string _fileName, int _cellId);
	void particleEnergy(std::string _fileName, int _bodyId);

	int fluidVtkCounter = 0;
	int solidVtkCounter = 0;
};

#endif // !OUTPUT_H
