#ifndef OUTPUT_H
#define OUTPUT_H

//Standard Library
#include <memory>
#include <vector>
#include <string>

//Hybrid Library
#include "Math.h"
#include "Scene.h"

class Output {
public:

	void fluidVTK(std::string _fileName);
	std::vector<std::shared_ptr<Lattice>> fluidInfo;
	
};

#endif //BODY_H