#ifndef RENDERSETTINGSH
#define RENDERSETTINGSH

#include <string>

using namespace std;

enum RenderMode
{
	RAYTRACE,
	PATHTRACE
};

enum SamplingType
{
	UNIFORM,
	IMPORTANCE
};

/* RenderSettings structs hold all the information necessary
   to render a Scene. */
typedef struct render_settings_struct {

	RenderMode mode;
	SamplingType sampleType;
	bool directLighting;
	bool indirectLighting;
	bool russianRoulette;
	string filename;
	int threads;
	unsigned int pixelWidth;
	unsigned int pixelHeight;
	unsigned int sqrtSamplesPerPixel;
	unsigned int recursionDepth;
	double rayBias;

} RenderSettings;


#endif