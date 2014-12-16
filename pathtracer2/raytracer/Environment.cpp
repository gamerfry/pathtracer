#include "Environment.h"


ConstantEnvironment::ConstantEnvironment(rgb color)
: bgColor(color)
{
}

rgb ConstantEnvironment::radiance(Ray& ray)
{
	return bgColor;
}


HDREnvironment::HDREnvironment(string mapfilename)
{
	if (mapfilename.find(".pfm") != string::npos)
		hdrMap = FreeImage_Load(FIF_PFM, mapfilename.c_str(), PFM_DEFAULT);
	else
	{
		cout << endl;
		cerr << "Error: Unsupported HDR map format in " << mapfilename << endl;
		exit(1);
	}

	if (!hdrMap)
	{
		cout << endl;
		cerr << "Error: HDR map \"" << mapfilename << "\" not found/loaded!" << endl;
		exit(1);
	}

	width = FreeImage_GetWidth(hdrMap);
	height = FreeImage_GetHeight(hdrMap);
	pitch = FreeImage_GetPitch(hdrMap);
	BPP = FreeImage_GetBPP(hdrMap);
}

HDREnvironment::~HDREnvironment()
{
	FreeImage_Unload(hdrMap);
}

rgb HDREnvironment::radiance(Ray& ray)
{
	// Direction --> Angular map coordinate conversion
	// Courtesy of Paul Debevec and Dan Lemmon's
	// SIGGRAPH 2001 Image-Based Lighting course notes

	vec3 dir = ray.getDirection();
	dir.normalize();

	double r = 0.159154943*acos(dir[2])/sqrt(dir[0]*dir[0] + dir[1]*dir[1]);
	double u = 0.5 + dir[0] * r;
	double v = 0.5 + dir[1] * r;

	// Now get the radiance out of the HDR map
	int col = (int)(u*width);
	int row = (int)((1-v)*height);
	rgb rad;
	FIRGBF* pixels = (FIRGBF*)FreeImage_GetBits(hdrMap);
	rad[0] = pixels[row*height + col].red;
	rad[1] = pixels[row*height + col].green;
	rad[2] = pixels[row*height + col].blue;

	return rad;
}