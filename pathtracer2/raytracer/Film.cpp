#include "Film.h"
#include "FreeImage.h"
#include <iostream>

using namespace std;


/* Constructors */

Film::Film(unsigned int imageWidth, unsigned int imageHeight, int sqrtNumSamples) {

	sampleGrid = new rgb*[imageWidth];
	for(unsigned int i = 0; i < imageWidth; i++)
		sampleGrid[i] = new rgb[imageHeight];

	pixelWidth = imageWidth;
	pixelHeight = imageHeight;

	numSamples = sqrtNumSamples*sqrtNumSamples;

}


/* Destructor */

Film::~Film() {

	for (unsigned int i = 0; i < pixelWidth; i++)
		delete [] sampleGrid[i];
	delete [] sampleGrid;

}


/* Instance methods */

void Film::commit(const Sample& samp, const rgb& color) {

	unsigned int i = (unsigned int)samp.horiz;
	unsigned int j = (unsigned int)samp.vert;
	sampleGrid[i][j] += color;

}

void Film::writeImage(string filename) {

	// [START] WRITE IMAGE
	cout << "Writing to file \"" << filename << "\"...";

	if (filename.find(".png") != string::npos)
	{
		writePng(filename);
	}
	else if (filename.find(".pfm") != string::npos)
	{
		writePfm(filename);
	}
	else
	{
		cout << endl;
		cerr << "Unsupported image output format specified by \"" << filename << "\"" << endl;
		exit(1);
	}

	// [END] WRITE IMAGE
	cout << "DONE" << endl;
}

void Film::writePng(string filename)
{
	FIBITMAP* image = FreeImage_Allocate(pixelWidth, pixelHeight, 24);
	RGBQUAD pixel;

	for (unsigned int i = 0; i < pixelWidth; i++)
	{
		for (unsigned int j = 0; j < pixelHeight; j++) {
			rgb pixelColor = sampleGrid[i][j] / numSamples;
			pixelColor.clamp();
			pixel.rgbRed = (BYTE)(pixelColor[0] * 255);
			pixel.rgbGreen = (BYTE)(pixelColor[1] * 255);
			pixel.rgbBlue = (BYTE)(pixelColor[2] * 255);
			FreeImage_SetPixelColor(image, i, j, &pixel);
		}
	}

	FreeImage_Save(FIF_PNG, image, filename.c_str(), PNG_IGNOREGAMMA);
}

void Film::writePfm(string filename)
{
	FIBITMAP* image = FreeImage_AllocateT(FIT_RGBF, pixelWidth, pixelHeight);
	FIRGBF* pixels = (FIRGBF*)FreeImage_GetBits(image);

	for (unsigned int j = 0; j < pixelHeight; j++)
	{
		for (unsigned int i = 0; i < pixelWidth; i++)
		{
			rgb pixelColor = sampleGrid[i][j] / numSamples;
			unsigned int line = pixelHeight - 1 - j;
			pixels[pixelWidth*line + i].red = (float)pixelColor[0];
			pixels[pixelWidth*line + i].green = (float)pixelColor[1];
			pixels[pixelWidth*line + i].blue = (float)pixelColor[2];
		}
	}
	
	FreeImage_Save(FIF_PFM, image, filename.c_str(), PFM_DEFAULT);

}