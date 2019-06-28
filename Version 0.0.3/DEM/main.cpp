#include "DEM.h"

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
	D.addBody(1, 0.5, { 3,9 }, { 0,0 });
	D.addBody(1, 0.5, { 7,9 }, { 0,0 });

	//Set simulation:
	D.set_TimeStep(0.5, 1e6);
	D.set_Boundary(xLim, yLim);
	D.demInfo();

	//Engine
	for (int i = 0; i != 30000; i++) {
		D.demEnergy();
		D.demCycle();
		if (i % 100 == 0)  D.outputSVTK("resultado");
	}
	D.outputECSV(30000, "Energia");

	return 0;
}
