#ifndef SCENEH
#define SCENEH

#include "Camera.h"
#include "Lights.h"
#include "RenderSettings.h"
#include "Primitives.h"
#include "Environment.h"
#include <string>
#include <vector>
#include <fstream>
#include <time.h>

using namespace std;

/* Represents a scene for the raytracer to render. */
class Scene {

private:

	/* Instance vars */
	Camera* sceneCam;					// Camera for this scene.
	vector<Light*> sceneLights;			// Lights for this scene.
	BoundingBoxTree* hierarchy;			// Object hierarchy for this scene.
	void openLog(const RenderSettings& settings);
	void closeLog(const RenderSettings& settings);
	static string convertTime(time_t atime);

public:

	/* Constructors */
	Scene(Camera* cam, Environment* env = new ConstantEnvironment(rgb::black));
	~Scene();
	/* Instance methods */
	void render(const RenderSettings& settings);
	void addLight(Light* light);
	void setHierarchy(BoundingBoxTree* tree);
	vector<Light*> getLights();
	BoundingBoxTree* getHierarchy();
	ofstream logfile;
	time_t starttime;
	Environment* environment;

};


#endif
