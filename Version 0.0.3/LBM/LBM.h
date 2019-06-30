#ifndef LBM_H
#define LBM_H

#include "Lattice.h"
#include <memory>

class LBM {
public:

	
	std::vector<std::shared_ptr<Lattice>> cells;
};
#endif // !LBM_H

