#ifndef _GRID_
#define _GRID_

#include<vector>

class Grid {
public:
	Grid(int _Nx = 100, int _Ny = 100) : Nx(_Nx), Ny(_Ny) {};

	int MapGrid(int i, int j);

	int MapFunction(int i, int j, int k);

	void setBoundary(bool _Top, bool _Bottom, bool _Left, bool _Right);

	int Nx;
	int Ny;
	int Nx1 = Nx + 1;
	int Ny1 = Ny + 1;

	int Boundary[101*101];
};


#endif // !_GRID_


