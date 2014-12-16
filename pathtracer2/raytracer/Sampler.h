#ifndef SAMPLERH
#define SAMPLERH

#include "randomc.h"

/* A simple way to describe a pixel sample in
   terms of the screen coordinates. */
typedef struct sample_struct {
	double horiz;
	double vert;
	unsigned int p;			// Which sample on the given pixel produced this?
	unsigned int q;
	unsigned int id;		// Unique sample id
} Sample;


/* Sampler objects enumerate through all the samples necessary to draw
   every pixel on the screen. Currently, the Sampler class only does one
   sample per pixel. This is not difficult to change. */
class Sampler {

private:

	/* Instance vars */
	unsigned int pixelWidth;				// Width of the screen
	unsigned int pixelHeight;				// Height of the screen
	unsigned int i, j;						// The next pixel to sample
	unsigned int p, q;						// The next part of the pixel to sample
	unsigned int n;							// n = sqrt(# of samples per pixel)
	unsigned int total;						// Total number of samples provided by THIS
	unsigned int accum;						// Number of samples generated so far
	unsigned int percent;					// Percentage of total samples generated so far
	CRandomMersenne* randGen;				// Random number generator

public:

	/* Constructors */
	Sampler(unsigned int width, unsigned int height, unsigned int perPixel);

	/* Destructor */
	~Sampler();

	/* Instance methods */
	inline bool hasMoreSamples() {
		return i < pixelWidth &&
			j < pixelHeight;
	}

	Sample nextSample();

	Sample normalizeSample(const Sample& samp);

};


/* Generates all the samples necessary for a single pixel */
class PixelSampler
{
private:
	unsigned int pixelWidth;				// Width of the screen
	unsigned int pixelHeight;				// Height of the screen
	CRandomMersenne* randGen;				// Random number generator
	unsigned int i, j;						// Pixel being sampled
	unsigned int p, q;						// The next part of the pixel to sample
	unsigned int n;							// n = sqrt(# of samples per pixel)
	unsigned int accum;						// Number of samples generated so far

public:
	PixelSampler(CRandomMersenne* random, unsigned int i, unsigned int j,
		         unsigned int width, unsigned int height, unsigned int perPixel);
	Sample nextSample();
	Sample normalizeSample(const Sample& samp);

	inline bool hasMoreSamples()
	{
		return p < n && q < n;
	}
};


#endif