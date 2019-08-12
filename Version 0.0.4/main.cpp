/*
#include "Scene.h"


int main() {
	
	Scene S({ 10,10 }, 1, 1, 1);
	
	//Parameters:
	S.D.gravity = { 0, -9.81 };
	S.D.localDamping = 0;
	S.D.frictionAngle = 30;

	//Body addition:
	S.addBody(1, 1, { 5,9 }, { 0,0 });
	//S.addBody(1, 1, { 3,9 }, { 0,0 });
	//D.addBody(1, 0.5, { 7,9 }, { 0,0 });

	//Set simulation:
	S.setTimeStep(0.2, 1e6);


	//Engine
	for (int i = 0; i != 30000; i++) {
		S.D.demEnergy();
		S.D.demCycle();
		if (i % 100 == 0)  S.D.outputSVTK("resultado");
	}
	S.D.outputECSV("Energia");

	return 0;
}
*/