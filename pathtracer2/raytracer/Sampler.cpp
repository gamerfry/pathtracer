#include "Sampler.h"
#include <time.h>
#include <cstdlib>
#include <iostream>

using namespace std;

/* Constructors */

Sampler::Sampler(unsigned int width, unsigned int height, unsigned int perPixel) {

	pixelWidth = width;
	pixelHeight = height;
	n = perPixel;
	i = j = 0;
	p = q = 0;
	total = width*height*perPixel*perPixel;
	accum = 0;
	percent = 0;

	randGen = new CRandomMersenne(0);

}


/* Destructor */

Sampler::~Sampler() {

	delete randGen;
}


/* Instance methods */

Sample Sampler::nextSample() {

	if (!hasMoreSamples())
		throw "Sampler has no more samples.";

	// Output rendering percentage?
	unsigned int newpercent = (int)((float)accum/total*100);
	if (newpercent > percent)
	{
		percent = newpercent;
		cout << "Percent Complete: " << percent << endl;
	}

	Sample toReturn;
	toReturn.id = accum;
	toReturn.p = p;
	toReturn.q = q;

	// Stratified (jittered) sampling (But only if n > 1).
	double eps1 = n == 1 ? 0.5 : randGen->Random();		//((double)rand()) / RAND_MAX;
	double eps2 = n == 1 ? 0.5 : randGen->Random();		//((double)rand()) / RAND_MAX;
	toReturn.horiz = i + (p + eps1) / n;
	toReturn.vert = j + (q + eps2) / n;

	// CASE: This is the last sample for the overall last pixel
	if (i == pixelWidth - 1 && j == pixelHeight - 1 && p == n - 1 && q == n - 1) {
		i++;
		j++;
		p = q = 0;
	}
	// CASE: This is the last sample for the last pixel in one row
	else if (i == pixelWidth - 1 && p == n - 1 && q == n - 1) {
		i = 0;
		j++;
		p = q = 0;
	}
	// CASE: This is the last sample for a pixel in the middle of a row
	else if (p == n - 1 && q == n - 1) {
		i++;
		p = q = 0;
	}
	// CASE: This is the last sample in one row of a single pixel
	else if (q == n - 1) {
		q = 0;
		p++;
	}
	// CASE: This is a sample in the middle of a row of a single pixel
	else q++;

	accum++;
	return toReturn;
	
}

Sample Sampler::normalizeSample(const Sample& samp) {

	Sample toReturn;
	toReturn.horiz = samp.horiz / pixelWidth;
	toReturn.vert = samp.vert / pixelHeight;
	toReturn.p = samp.p;
	toReturn.q = samp.q;

	return toReturn;
}




PixelSampler::PixelSampler(CRandomMersenne* random, unsigned int i, unsigned int j,
		                   unsigned int width, unsigned int height, unsigned int perPixel)
{
	randGen = random;
	pixelWidth = width;
	pixelHeight = height;
	n = perPixel;
	accum = 0;
	this->i = i;
	this->j = j;
	p = q = 0;
}

Sample PixelSampler::nextSample()
{
	if (!hasMoreSamples())
		throw "Sampler has no more samples.";

	Sample toReturn;
	toReturn.id = accum;
	toReturn.p = p;
	toReturn.q = q;

	// Stratified (jittered) sampling (But only if n > 1).
	double eps1 = n == 1 ? 0.5 : randGen->Random();		//((double)rand()) / RAND_MAX;
	double eps2 = n == 1 ? 0.5 : randGen->Random();		//((double)rand()) / RAND_MAX;
	toReturn.horiz = i + (p + eps1) / n;
	toReturn.vert = j + (q + eps2) / n;

	// CASE: This is the last sample
	if (p == n - 1 && q == n - 1)
	{
		p++;
		q++;
	}

	// CASE: This is the last sample in one row of a single pixel
	if (q == n - 1) {
		q = 0;
		p++;
	}
	// CASE: This is a sample in the middle of a row of a single pixel
	else q++;

	accum++;
	return toReturn;
}

Sample PixelSampler::normalizeSample(const Sample& samp)
{
	Sample toReturn;
	toReturn.horiz = samp.horiz / pixelWidth;
	toReturn.vert = samp.vert / pixelHeight;
	toReturn.p = samp.p;
	toReturn.q = samp.q;

	return toReturn;
}