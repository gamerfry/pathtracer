#ifndef ENVIRONMENTH
#define ENVIRONMENTH

#include "rgb.h"
#include "Ray.h"
#include "FreeImage.h"
#include <string>

using namespace std;

/* Abstract base class for all environments */
class Environment
{
public:
	virtual rgb radiance(Ray& ray) = 0;
};


/* Environments that are just a constant background color */
class ConstantEnvironment : public Environment
{
public:
	ConstantEnvironment(rgb color);
	rgb radiance(Ray& ray);

private:
	rgb bgColor;
};


/* High dynamic range environments */
class HDREnvironment : public Environment
{
public:
	HDREnvironment(string mapfilename);
	~HDREnvironment();
	rgb radiance(Ray& ray);

private:
	FIBITMAP* hdrMap;

	int width;
	int height;
	int pitch;
	int BPP;
};


#endif		// ENVIRONMENT_H