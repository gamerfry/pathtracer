#include "Scene.h"
#include "Sampler.h"
#include "Film.h"
#include "RayTracer.h"
#include "rgb.h"
#include <iostream>
#include <sstream>
#include <omp.h>

using namespace std;


/* Constructors */

Scene::Scene(Camera* cam, Environment* env) {

	sceneCam = cam;
	environment = env;
}

Scene::~Scene() {

	delete sceneCam;
	delete environment;
}


/* Instance methods */

void Scene::render(const RenderSettings& settings) {

	// [START] RENDER
	cout << "Rendering..." << endl;
	openLog(settings);

	Film output(settings.pixelWidth, settings.pixelHeight, settings.sqrtSamplesPerPixel);
	RayTracer tracer(this, settings);

	// Single-threaded version
	//Sampler samples(settings.pixelWidth, settings.pixelHeight, settings.sqrtSamplesPerPixel);
	//while (samples.hasMoreSamples()) {
	//	Sample s = samples.nextSample();
	//	Ray viewRay = sceneCam->createViewingRay(samples.normalizeSample(s));
	//	rgb pixelColor = tracer.traceViewingRay(viewRay);
	//	output.commit(s, pixelColor);
	//}

	// Multi-threaded version
	int totalPixels = settings.pixelHeight*settings.pixelWidth;
	int pixelsDone = 0;
	int percent = 0;
	CRandomMersenne random(0);
	omp_set_num_threads(settings.threads);
	#pragma omp parallel for
	for (int p = 0; p < totalPixels; p++)
	{
		unsigned int i = p / settings.pixelWidth;
		unsigned int j = p % settings.pixelWidth;
		PixelSampler samples(&random, i, j, settings.pixelWidth, settings.pixelHeight, settings.sqrtSamplesPerPixel);
		while (samples.hasMoreSamples())
		{
			Sample s = samples.nextSample();
			Ray viewRay = sceneCam->createViewingRay(samples.normalizeSample(s));
			rgb pixelColor = tracer.traceViewingRay(viewRay);
			output.commit(s, pixelColor);
		}

		// Output percent complete
		pixelsDone++;
		int newpercent = (int)((float)pixelsDone/totalPixels*100);
		if (newpercent > percent)
		{
			percent = newpercent;
			cout << "Percent Complete: " << percent << endl;
		}
	}

	// [END] RENDER
	cout << "Rendering DONE" << endl;

	output.writeImage(settings.filename);
	closeLog(settings);
}

void Scene::addLight(Light* light) {

	sceneLights.push_back(light);

}

void Scene::setHierarchy(BoundingBoxTree* tree) {

	hierarchy = tree;
}

vector<Light*> Scene::getLights() {

	return sceneLights;

}

BoundingBoxTree* Scene::getHierarchy() {

	return hierarchy;

}

void Scene::openLog(const RenderSettings& settings)
{
	starttime = time(NULL);
	logfile.open((settings.filename + "_log.txt").c_str());
	logfile << "Rendering image " << settings.filename << "..." << endl;
	logfile << "Resolution: " << settings.pixelWidth << "x" << settings.pixelHeight << endl;
	logfile << "Paths Per Pixel: " << settings.sqrtSamplesPerPixel*settings.sqrtSamplesPerPixel << endl;
	logfile << "Threads: " << settings.threads << endl;
	logfile << "BRDF Sampling: " << (settings.sampleType == UNIFORM ? "Uniform" : "Importance") << endl;
	logfile << "Direct Light Sampling: " << (settings.directLighting ? "Enabled" : "Disabled") << endl;
	logfile << "Indirect Illumination: " << (settings.indirectLighting ? "Enabled" : "Disabled") << endl;
	logfile << "Russian Roulette: " << (settings.russianRoulette ? "Enabled" : "Disabled") << endl;
	if (settings.russianRoulette)
		logfile << "Minimum Path Length: " << settings.recursionDepth << endl;
	else logfile << "Maximum Recursion Depth: " << settings.recursionDepth << endl;
}

void Scene::closeLog(const RenderSettings& settings)
{
	time_t endtime = time(NULL);
	logfile << "Finished rendering after " << convertTime(endtime-starttime) << endl;
	logfile.close();
}

string Scene::convertTime(time_t atime)
{
	time_t hours = atime/3600;
	atime -= hours*3600;
	time_t minutes = atime/60;
	atime -= minutes*60;
	time_t seconds = atime;
	ostringstream ss;
	ss << hours << ":" << minutes << ":" << seconds;
	return ss.str();
}