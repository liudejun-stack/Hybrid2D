#include "Lattice.hpp"

int Lattice::MapGrid(int i, int j)
{
	return nx * j + i;
}

int Lattice::MapFunction(int i, int j, int k)
{
	return nx * (ny*k + j) + i;
}

void Lattice::setBoundary(bool _Top, bool _Bottom, bool _Left, bool _Right)
{
	for (int j = 0; j < ny; j++) {
		for (int i = 0; i < nx; i++) {
			Boundary[MapGrid(i, j)] = 0;
		}
	}

	if (_Top) {
		for (int i = 0; i < nx; i++) {
			Boundary[MapGrid(i, ny)] = 1;
		}
	}

	if (_Bottom) {
		for (int i = 0; i < nx; i++) {
			Boundary[MapGrid(i, 0)] = 1;
		}
	}

	if (_Left) {
		for (int j = 0; j < ny; j++) {
			Boundary[MapGrid(0, j)] = 1;
		}
	}

	if (_Right) {
		for (int j = 0; j < ny; j++) {
			Boundary[MapGrid(nx, j)] = 1;
		}
	}
}