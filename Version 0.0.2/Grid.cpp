#include "Grid.hpp"

int Grid::MapGrid(int i, int j)
{
	return Nx * j + i;
}

int Grid::MapFunction(int i, int j, int k)
{
	return Nx * (Ny*k + j) + i;
}

void Grid::setBoundary(bool _Top, bool _Bottom, bool _Left, bool _Right)
{
	for (int j = 0; j <= Ny; j++) {
		for (int i = 0; i <= Nx; i++) {
			Boundary[MapGrid(i, j)] = 0;
		}
	}

	if (_Top) {
		for (int i = 0; i <= Nx; i++) {
			Boundary[MapGrid(i, Ny)] = 1;
		}
	}

	if (_Bottom) {
		for (int i = 0; i <= Nx; i++) {
			Boundary[MapGrid(i, 0)] = 1;
		}
	}

	if (_Left) {
		for (int j = 0; j <= Ny; j++) {
			Boundary[MapGrid(0, j)] = 1;
		}
	}

	if (_Right) {
		for (int j = 0; j <= Ny; j++) {
			Boundary[MapGrid(Nx, j)] = 1;
		}
	}
}


