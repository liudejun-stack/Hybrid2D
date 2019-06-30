#include "Math.h"
#include <vector>
#include <iostream>
#include "Lattice.h"

int main() {
	
	Lattice L(0, 1, 2);
	for (int i = 0; i < L.f.size(); i++) {
		L.f[i] = 5;
		std::cout << L.f[i] << std::endl;
	}
}