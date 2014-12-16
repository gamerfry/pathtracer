#include "rgb.h"


/* Special functions */
rgb& rgb::clamp() {

	n[VX] = MIN(1.0, n[VX]);
	n[VY] = MIN(1.0, n[VY]);
	n[VZ] = MIN(1.0, n[VZ]);

	n[VX] = MAX(0.0, n[VX]);
	n[VY] = MAX(0.0, n[VY]);
	n[VZ] = MAX(0.0, n[VZ]);

	return *this;

}

rgb& rgb::normalize()
{
	double maxcomp = MAX(n[0], MAX(n[1], n[2]));
	n[0] /= maxcomp;
	n[1] /= maxcomp;
	n[2] /= maxcomp;

	return *this;
}

const rgb rgb::black = rgb(0,0,0);