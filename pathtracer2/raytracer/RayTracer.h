#ifndef RAYTRACERH
#define RAYTRACERH

#include "algebra3.h"
#include "Scene.h"
#include "Ray.h"
#include "Primitives.h"
#include "IntersectRecord.h"

/* Raytracer objects trace viewing rays into the
   scene in order to determine sample color. */
class RayTracer {

private:

	/* Instance vars */
	RenderSettings settings;
	Scene* tracingScene;
	CRandomMersenne* random;

	/* Instance methods */

	// Tracing
	rgb trace(Ray& ray, unsigned int depth, bool indirect=false);
	rgb raytrace(Ray& ray, unsigned int depth, bool indirect=false);
	rgb pathtrace(Ray& ray, unsigned int depth, bool indirect=false);
	bool traceShadowRay(Ray& ray);

	// Monte Carlo utility functions
	vec3 sampleUpperHemisphere(vec3& alignWithZ, double n);
	vec3 uniformSampleUpperHemisphere(vec3& alignWithZ);
	vec3 importanceSampleUpperHemisphere(vec3& alignWithZ, double n);
	bool russianRoulette(const rgb& refl, double& survivorMult);
    bool glossyRussianRoulette(const rgb& kS, const rgb&kD, double& survivorMult);


	// Direct illumination
	rgb directIllumination(const IntersectRecord& intersection, Ray& ray);
	rgb diffComp(const IntersectRecord& intersection, const vec3& incidence, const rgb& color);
	rgb specComp(const IntersectRecord& intersection, const vec3& incidence, const rgb& color, Ray& viewRay);
	rgb directIllum(Ray& ray, IntersectRecord& intersection);

	// Indirect illumination
	rgb diffuseInterreflect(Ray& ray, IntersectRecord& intersection, int depth);
    rgb specularInterreflect(Ray& ray, IntersectRecord& intersection, int depth);
	rgb mirrorReflect(Ray& ray, IntersectRecord& intersection, int depth);
	bool dielectricCalc(Ray& ray, IntersectRecord& intersection, Ray& refrRay, double& schlick);
	rgb dielectricTransmit(Ray& ray, IntersectRecord& intersection, int depth);
	rgb dieletricRefract(Ray& ray, IntersectRecord& intersection, int depth, rgb reflColor);

	// Geometric utility functions
	vec3 reflect(vec3& dir, const vec3& normal);
    bool refract(Ray& ray, const IntersectRecord& intersect, double oldIndex, double newIndex, vec3& refractDirection);
	double schlickCalc(double n, double nt, vec3& rayDir, vec3& refrDir, vec3& surfNorm);

public:

	/* Constructors */
	RayTracer(Scene* scene, RenderSettings settings);

	/* Destructor */
	~RayTracer();

	/* Instance methods */
	rgb traceViewingRay(Ray& ray);

};


#endif