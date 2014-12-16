#ifndef FILMH
#define FILMH

#include "rgb.h"
#include "Sampler.h"
#include <string>
#include <vector>

using namespace std;

/* Film objects collect samples to eventually write to a
   file. */
class Film {

private:

	/* Instance vars */
	unsigned int pixelWidth;				// Image width in pixels
	unsigned int pixelHeight;				// Image width in pixels
	rgb** sampleGrid;						// Stores accumulated samples
	int numSamples;							// Number of samples per pixel

	void writePng(string filename);
	void writePfm(string filename);

public:

	/* Constructors */
	Film(unsigned int imageWidth, unsigned int imageHeight, int sqrtNumSamples);

	/* Destructor */
	~Film();

	/* Instance methods */
	void commit(const Sample& samp, const rgb& color);		// Stores one sample
	void writeImage(string filename);						// Writes to a file
};


#endif