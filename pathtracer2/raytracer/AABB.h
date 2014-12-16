#ifndef AABBH
#define AABBH
#include <vector>


class AABB {
public:
	int _numDimensions;
	std::vector<double> min;
	std::vector<double> max;
	double EPSILON = 1e-16;
	AABB(std::vector<double> min, std::vector<double> max);
};
#endif