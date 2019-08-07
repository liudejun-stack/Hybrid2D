/*#include "DEM.h"
#include "LBM.h"


int main() {
	
	
	DEM D;
	
	//Parameters:
	D.gravity = { 0, -9.81 };
	D.localDamping = 0;
	D.frictionAngle = 30;
	Vec2i xLim = { 0, 10 };
	Vec2i yLim = { 0, 10 };

	//Body addition:
	D.addBody(1, 0.5, { 5,9 }, { 0,0 });
	D.addBody(1, 1, { 3,9 }, { 0,0 });
	//D.addBody(1, 0.5, { 7,9 }, { 0,0 });

	//Set simulation:
	D.setTimeStep(0.5, 1e6);
	D.setBoundary(xLim, yLim);

	//Engine
	for (int i = 0; i != 30000; i++) {
		D.demEnergy();
		D.demCycle();
		if (i % 100 == 0)  D.outputSVTK("resultado");
	}
	D.outputECSV("Energia");

	for (auto& B : D.bodies) {
		std::cout << B->functionR << std::endl;;
	}
	
	return 0;
	
}
*/